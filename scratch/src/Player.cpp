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

    m_Shape = b3CreateHullShape(m_Body, &shapeDef, &hull.base);

    SyncCameraToBody();
}

Player::~Player()
{
    delete m_PlayerCamera;
}

float Player::CalculateJumpHeight()
{
    switch (m_PlayerState)
    {
        case Crouching:
            return CROUCHING_JUMP_HEIGHT;
        case Standing:
            return (m_IsCrouched)
                ? STANDING_JUMP_CROUCH_HEIGHT
                : STANDING_JUMP_HEIGHT;
        case Walking:
            return (m_IsCrouched)
                ? WALKING_JUMP_CROUCH_HEIGHT
                : WALKING_JUMP_HEIGHT;
    }
}

void Player::Jump()
{
    float jumpHeight = CalculateJumpHeight();
    b3Vec3 velocity = b3Body_GetLinearVelocity(m_Body);

    velocity.y = sqrt(2 * GRAVITY * jumpHeight);

    b3Body_SetLinearVelocity(m_Body, velocity);
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

    b3Vec3 velocity = b3Body_GetLinearVelocity(m_Body);
    m_PlayerState = m_IsCrouched
        ? Crouching
        : (velocity.x == 0.0f && velocity.z == 0.0f)
            ? Standing
            : Walking;
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

void Player::SyncCameraToBody()
{
    float halfHeight = m_IsCrouched ? HALF_HEIGHT_CROUCH : HALF_HEIGHT;

    Vector3 center = ToVector3(b3Body_GetPosition(m_Body));
    Vector3 feet = center - Vector3(0.0f, halfHeight, 0.0f);

    m_PlayerCamera->SetPosition(feet + Vector3(0.0f, m_EyeLevel, 0.0f));
}

void Player::Update(Keyboard::State kbState, float dt)
{
    if (kbState.Left) { m_PlayerCamera->RotateLeft(dt); }
    if (kbState.Right) { m_PlayerCamera->RotateRight(dt); }
    if (kbState.Up) { m_PlayerCamera->RotateUp(dt); }
    if (kbState.Down) { m_PlayerCamera->RotateDown(dt); }

    if ((kbState.LeftControl && !m_IsCrouched) 
        || (!kbState.LeftControl && m_IsCrouched)) { Crouch(); }

    if (kbState.W || kbState.S || kbState.D || kbState.A)
    {
        constexpr float FRICTION = 4.0f;
        constexpr float STOP_SPEED = 100.0f;
        constexpr float ACCELERATE = 10.0f;

        Vector3 forward = m_PlayerCamera->GetForward();
        forward.y = 0.0f;   // ground movement ignores pitch
        forward.Normalize();

        Vector3 right = m_PlayerCamera->GetRight();
        right.y = 0.0f;
        right.Normalize();

        Vector3 wishDir = Vector3::Zero;
        if (kbState.W) { wishDir += forward; }
        if (kbState.S) { wishDir -= forward; }
        if (kbState.D) { wishDir += right; }
        if (kbState.A) { wishDir -= right; }
        if (wishDir.LengthSquared() > 0.0f) { wishDir.Normalize(); }

        Vector3 velocity = ToVector3(b3Body_GetLinearVelocity(m_Body));

        float speed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

        if (speed >= 0.001f)
        {
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

        float currentSpeed = velocity.Dot(wishDir);
        float addSpeed = m_WishSpeed - currentSpeed;

        if (addSpeed > 0.0f)
        {
            float accelSpeed = ACCELERATE * dt * MOVEMENT_SPEED_MAX;
            accelSpeed = (accelSpeed < addSpeed) ? accelSpeed : addSpeed;
            velocity += wishDir * accelSpeed;
        }

        b3Body_SetLinearVelocity(m_Body, ToB3Vec3(velocity));
    }

    if (kbState.Space && IsGrounded()) { Jump(); }

    SyncCameraToBody();
}
