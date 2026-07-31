#include <algorithm>
#include <cmath>

#include "Player.h"


Player::Player(b3WorldId physicsWorld, Vector3 spawnPosition) :
    m_PhysicsWorld(physicsWorld),
    m_SpawnPosition(spawnPosition)
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

    Vector3 center = ToVector3(b3Body_GetPosition(m_Body));
    Vector3 feet = center - Vector3(0.0f, HALF_HEIGHT - GROUND_CHECK_DISTANCE * 0.5f, 0.0f);

    b3BoxHull box = b3MakeBoxHull(
        HALF_WIDTH - 1.0f,
        GROUND_CHECK_DISTANCE * 0.5f,
        HALF_DEPTH - 1.0f
    );

    b3ShapeProxy proxy;
    proxy.points = box.boxPoints;
    proxy.count = 8;
    proxy.radius = 0.0f;

    bool result = false;

    b3World_OverlapShape(
        m_PhysicsWorld, 
        b3ToPos(ToB3Vec3(feet)), 
        &proxy,
        filter,
        [](b3ShapeId, void* context) -> bool
        {
            *static_cast<bool*>(context) = true;
            return false; // stop at the first hit
        },
        &result
    );

    return result;
}

void Player::ApplyFriction(Vector3& velocity, float dt)
{
    float speed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

    if (speed < 0.001f) return;

    // Prevent the player from sliding forever at low speeds.
    float control = (speed > STOP_SPEED) ? speed : STOP_SPEED;

    // Amount of speed to remove this frame.
    float drop = control * FRICTION * dt;

    float newSpeed = speed - drop;
    if (newSpeed < 0) newSpeed = 0;

    velocity.x *= newSpeed / speed;
    velocity.z *= newSpeed / speed;
}

void Player::Jump(Vector3& velocity)
{
    velocity.y = JUMP_VELOCITY;
}

void Player::SyncCameraToBody()
{
    Vector3 center = ToVector3(b3Body_GetPosition(m_Body));
    Vector3 feet = center - Vector3(0.0f, HALF_HEIGHT, 0.0f);

    m_PlayerCamera->SetPosition(feet + Vector3(0.0f, EYE_LEVEL, 0.0f));
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

        // Forward/back/strafe are summed unnormalized, so a diagonal wish vector skews toward
        // whichever input is stronger rather than splitting evenly between the two.
        float forwardMove = 0.0f;
        if (kbState.W) { forwardMove += FORWARD_SPEED; }
        if (kbState.S) { forwardMove -= BACK_SPEED; }

        float sideMove = 0.0f;
        if (kbState.D) { sideMove += SIDE_SPEED; }
        if (kbState.A) { sideMove -= SIDE_SPEED; }

        Vector3 wishVel = forward * forwardMove + right * sideMove;
        Vector3 wishDir = wishVel;
        float wishSpeed = wishDir.Length();
        if (wishSpeed > 0.0f) { wishDir /= wishSpeed; }

        if (wishSpeed > MAX_SPEED) { wishSpeed = MAX_SPEED; }

        float accelWishSpeed = grounded ? wishSpeed : (AIR_SPEED_CAP < wishSpeed ? AIR_SPEED_CAP : wishSpeed);
        float currentSpeed = velocity.Dot(wishDir);
        float addSpeed = accelWishSpeed - currentSpeed;

        if (addSpeed > 0.0f)
        {
            float accelerate = grounded ? GROUND_ACCELERATE : AIR_ACCELERATE;
            float accelSpeed = accelerate * dt * wishSpeed;
            accelSpeed = (accelSpeed < addSpeed) ? accelSpeed : addSpeed;
            velocity += wishDir * accelSpeed;
        }
    }

    if (kbState.Space && !m_JumpHeld && grounded) { Jump(velocity); }
    m_JumpHeld = kbState.Space;

    velocity.x = std::clamp(velocity.x, -MAX_VELOCITY, MAX_VELOCITY);
    velocity.y = std::clamp(velocity.y, -MAX_VELOCITY, MAX_VELOCITY);
    velocity.z = std::clamp(velocity.z, -MAX_VELOCITY, MAX_VELOCITY);

    b3Body_SetLinearVelocity(m_Body, ToB3Vec3(velocity));
    SyncCameraToBody();
}

void Player::Teleport(Vector3 position)
{
    b3Body_SetTransform(m_Body, ToB3Vec3(position), b3Quat_identity);
    b3Body_SetLinearVelocity(m_Body, b3Vec3{ 0.0f, 0.0f, 0.0f });

    SyncCameraToBody();
}

void Player::Update(Keyboard::State kbState, float dt)
{
    RotateCamera(kbState, dt);
    HandleMovement(kbState, dt);

    if (kbState.R) { Teleport(m_SpawnPosition); } // teleport player to starting position
}
