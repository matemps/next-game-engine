#include <cmath>

#include "Player.h"


Player::Player(b3WorldId physicsWorld, Vector3 spawnPosition) :
    m_PhysicsWorld(physicsWorld)
{
    b3BodyDef bodyDef = b3DefaultBodyDef();
    bodyDef.type = b3_dynamicBody;
    bodyDef.position = ToB3Vec3(spawnPosition);
    bodyDef.motionLocks.angularX = true;
    bodyDef.motionLocks.angularY = true;
    bodyDef.motionLocks.angularZ = true;
    bodyDef.enableSleep = false;

    m_Body = b3CreateBody(m_PhysicsWorld, &bodyDef);

    b3BoxHull hull = b3MakeBoxHull(HALF_WIDTH, HALF_HEIGHT, HALF_DEPTH);

    b3ShapeDef shapeDef = b3DefaultShapeDef();
    shapeDef.filter.categoryBits = c_PlayerCollisionCategory;

    // Friction is handled by ApplyFriction.
    shapeDef.baseMaterial.friction = 0.0f;

    m_Shape = b3CreateHullShape(m_Body, &shapeDef, &hull.base);

    SyncCameraToBody();
}

Player::~Player()
{
    delete m_PlayerCamera;
}

void Player::RotateCamera(Keyboard::State kbState, float dt)
{
    if (kbState.Up) { m_PlayerCamera->RotateUp(dt); }
    if (kbState.Down) { m_PlayerCamera->RotateDown(dt); }
    if (kbState.Left) { m_PlayerCamera->RotateLeft(dt); }
    if (kbState.Right) { m_PlayerCamera->RotateRight(dt); }
}

bool Player::IsGrounded()
{
    b3QueryFilter filter = b3DefaultQueryFilter();
    filter.maskBits = c_StaticCollisionCategory;

    float halfHeight = m_IsCrouched ? HALF_HEIGHT_CROUCH : HALF_HEIGHT;

    // Cast a short ray from the body center down to the feet - if it hits static
    // world geometry the player is standing on something and may jump.
    b3RayResult result = b3World_CastRayClosest(
        m_PhysicsWorld,
        b3Body_GetPosition(m_Body),
        b3Vec3{ 0.0f, -halfHeight, 0.0f },
        filter
    );

    return result.hit;
}

void Player::ApplyFriction(Vector3& velocity, float dt)
{
    float speed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

    if (speed < 0.001f) return;

    // Prevent the player from sliding forever at low speeds.
    float control = (speed > STOP_SPEED) ? speed : STOP_SPEED;

    // Amount of speed to remove this frame.
    float drop = control * FRICTION * dt;

    float newSpeed = (0.0f > (speed - drop)) ? 0.0f : (speed - drop);

    // Scale the horizontal velocity.
    newSpeed /= speed;

    velocity.x *= newSpeed;
    velocity.z *= newSpeed;
}

void Player::Jump(Vector3& velocity)
{
    float jumpHeight = 0.0f;

    switch (m_PlayerState)
    {
        case Crouching:
            jumpHeight = CROUCHING_JUMP_HEIGHT;
            break;
        case Standing:
            jumpHeight = (m_IsCrouched)
                ? STANDING_JUMP_CROUCH_HEIGHT
                : STANDING_JUMP_HEIGHT;
            break;
        case Walking:
            jumpHeight = (m_IsCrouched)
                ? WALKING_JUMP_CROUCH_HEIGHT
                : WALKING_JUMP_HEIGHT;
            break;
    }

    velocity.y = sqrt(2 * GRAVITY * jumpHeight);
}

void Player::Crouch()
{
    m_IsCrouched = !m_IsCrouched;

    m_EyeLevel = m_IsCrouched ? EYE_LEVEL_CROUCH : EYE_LEVEL;
    m_WishSpeed = m_IsCrouched ? MOVEMENT_SPEED_CROUCH : MOVEMENT_SPEED_WALK;

    b3BoxHull hull = m_IsCrouched
        ? b3MakeBoxHull(HALF_WIDTH, HALF_HEIGHT_CROUCH, HALF_DEPTH)
        : b3MakeBoxHull(HALF_WIDTH, HALF_HEIGHT, HALF_DEPTH);

    b3Shape_SetHull(m_Shape, &hull.base);
}

void Player::SyncCameraToBody()
{
    float halfHeight = m_IsCrouched ? HALF_HEIGHT_CROUCH : HALF_HEIGHT;

    Vector3 center = ToVector3(b3Body_GetPosition(m_Body));
    Vector3 feet = center - Vector3(0.0f, halfHeight, 0.0f);

    m_PlayerCamera->SetPosition(feet + Vector3(0.0f, m_EyeLevel, 0.0f));
}

void Player::SetPlayerState()
{
    Vector3 velocity = ToVector3(b3Body_GetLinearVelocity(m_Body));

    m_PlayerState = m_IsCrouched
        ? Crouching
        : (velocity.x == 0.0f && velocity.z == 0.0f)
            ? Standing
            : Walking;
}

void Player::HandleMovement(Keyboard::State kbState, float dt)
{
    Vector3 velocity = ToVector3(b3Body_GetLinearVelocity(m_Body));
    bool grounded = IsGrounded();
    
    if (grounded) { ApplyFriction(velocity, dt); }

    if (kbState.W || kbState.A || kbState.S || kbState.D)
    {
        Vector3 forward = m_PlayerCamera->GetForward();
        forward.y = 0.0f;   // ignore pitch
        forward.Normalize();

        Vector3 right = m_PlayerCamera->GetRight();
        right.y = 0.0f;     // ignore pitch
        right.Normalize();

        Vector3 wishDir = Vector3::Zero;
        if (kbState.W) { wishDir += forward; }
        if (kbState.S) { wishDir -= forward; }
        if (kbState.D) { wishDir += right; }
        if (kbState.A) { wishDir -= right; }
        if (wishDir.LengthSquared() > 0.0f) { wishDir.Normalize(); }

        float wishSpeed = grounded ? m_WishSpeed : (AIR_SPEED_CAP < m_WishSpeed ? AIR_SPEED_CAP : m_WishSpeed);
        float currentSpeed = velocity.Dot(wishDir);
        float addSpeed = wishSpeed - currentSpeed;

        if (addSpeed > 0.0f)
        {
            float accelerate = grounded ? GROUND_ACCELERATE : AIR_ACCELERATE;
            float accelSpeed = accelerate * dt * MOVEMENT_SPEED_MAX;
            accelSpeed = (accelSpeed < addSpeed) ? accelSpeed : addSpeed;
            velocity += wishDir * accelSpeed;
        }
    }

    if (kbState.Space && grounded) { Jump(velocity); }

    if ((kbState.LeftControl && !m_IsCrouched) 
        || (!kbState.LeftControl && m_IsCrouched)) { Crouch(); }

    
    
    b3Body_SetLinearVelocity(m_Body, ToB3Vec3(velocity));
    SyncCameraToBody();
    SetPlayerState();
}

void Player::Update(Keyboard::State kbState, float dt)
{
    RotateCamera(kbState, dt);
    HandleMovement(kbState, dt);
}
