#include <cmath>
#include <iostream>

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

namespace
{
    bool OnGroundOverlap(b3ShapeId, void* context)
    {
        *static_cast<bool*>(context) = true;
        return false; // stop at the first hit
    }
}

bool Player::IsGrounded()
{
    b3QueryFilter filter = b3DefaultQueryFilter();
    filter.maskBits = c_StaticCollisionCategory;

    float halfHeight = m_IsCrouched ? HALF_HEIGHT_CROUCH : HALF_HEIGHT;
    Vector3 center = ToVector3(b3Body_GetPosition(m_Body));

    float insetWidth = HALF_WIDTH - GROUND_CHECK_SKIN;
    float insetDepth = HALF_DEPTH - GROUND_CHECK_SKIN;
    float yLower = -halfHeight - GROUND_CHECK_SKIN;
    float yUpper = -halfHeight + GROUND_CHECK_SKIN;

    b3Vec3 points[8] = {
        { -insetWidth, yLower, -insetDepth },
        {  insetWidth, yLower, -insetDepth },
        { -insetWidth, yLower,  insetDepth },
        {  insetWidth, yLower,  insetDepth },
        { -insetWidth, yUpper, -insetDepth },
        {  insetWidth, yUpper, -insetDepth },
        { -insetWidth, yUpper,  insetDepth },
        {  insetWidth, yUpper,  insetDepth },
    };

    b3ShapeProxy proxy;
    proxy.points = points;
    proxy.count = 8;
    proxy.radius = 0.0f;

    bool grounded = false;
    b3World_OverlapShape(m_PhysicsWorld, b3ToPos(ToB3Vec3(center)), &proxy, filter, OnGroundOverlap, &grounded);

    return grounded;
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
    bool isMoving = velocity.x != 0.0f || velocity.z != 0.0f;

    float jumpHeight = m_IsCrouched
        ? CROUCHING_JUMP_HEIGHT
        : (isMoving ? WALKING_JUMP_HEIGHT : STANDING_JUMP_HEIGHT);

    velocity.y = sqrt(2 * GRAVITY * jumpHeight);
}

void Player::Crouch(bool grounded)
{
    float oldHalfHeight = m_IsCrouched ? HALF_HEIGHT_CROUCH : HALF_HEIGHT;

    m_IsCrouched = !m_IsCrouched;

    m_TargetEyeLevel = m_IsCrouched ? EYE_LEVEL_CROUCH : EYE_LEVEL;
    m_WishSpeed = m_IsCrouched ? MOVEMENT_SPEED_CROUCH : MOVEMENT_SPEED_WALK;

    float newHalfHeight = m_IsCrouched ? HALF_HEIGHT_CROUCH : HALF_HEIGHT;

    b3BoxHull hull = m_IsCrouched
        ? b3MakeBoxHull(HALF_WIDTH, HALF_HEIGHT_CROUCH, HALF_DEPTH)
        : b3MakeBoxHull(HALF_WIDTH, HALF_HEIGHT, HALF_DEPTH);

    b3Shape_SetHull(m_Shape, &hull.base);

    if (grounded)
    {
        Vector3 position = ToVector3(b3Body_GetPosition(m_Body));
        position.y += newHalfHeight - oldHalfHeight;
        b3Body_SetTransform(m_Body, ToB3Vec3(position), b3Quat_identity);
    }
    else
    {
        m_CameraFeetOffset -= oldHalfHeight - newHalfHeight;
    }
}

void Player::UpdateEyeLevel(float dt)
{
    float diff = m_TargetEyeLevel - m_EyeLevel;
    float step = EYE_LEVEL_TRANSITION_SPEED * dt;

    m_EyeLevel += (fabs(diff) <= step) ? diff : copysign(step, diff);
}

void Player::UpdateCameraFeetOffset(float dt)
{
    float step = EYE_LEVEL_TRANSITION_SPEED * dt;
    std::cout << step << std::endl;

    m_CameraFeetOffset = (fabs(m_CameraFeetOffset) <= step)
        ? 0.0f
        : m_CameraFeetOffset - copysign(step, m_CameraFeetOffset);
}

void Player::SyncCameraToBody()
{
    float halfHeight = m_IsCrouched ? HALF_HEIGHT_CROUCH : HALF_HEIGHT;

    Vector3 center = ToVector3(b3Body_GetPosition(m_Body));
    Vector3 feet = center - Vector3(0.0f, halfHeight, 0.0f) + Vector3(0.0f, m_CameraFeetOffset, 0.0f);

    m_PlayerCamera->SetPosition(feet + Vector3(0.0f, m_EyeLevel, 0.0f));
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

    if ((kbState.LeftControl && !m_IsCrouched)
        || (!kbState.LeftControl && m_IsCrouched)) { Crouch(grounded); }

    if (kbState.Space && !m_JumpHeld && grounded) { Jump(velocity); }
    m_JumpHeld = kbState.Space;

    b3Body_SetLinearVelocity(m_Body, ToB3Vec3(velocity));
    UpdateEyeLevel(dt);
    UpdateCameraFeetOffset(dt);
    SyncCameraToBody();
}

void Player::Teleport(Vector3 position)
{
    b3Body_SetTransform(m_Body, ToB3Vec3(position), b3Quat_identity);
    b3Body_SetLinearVelocity(m_Body, b3Vec3{ 0.0f, 0.0f, 0.0f });

    m_EyeLevel = m_TargetEyeLevel; // snap, don't blend across a teleport
    m_CameraFeetOffset = 0.0f;
    SyncCameraToBody();
}

void Player::Update(Keyboard::State kbState, float dt)
{
    RotateCamera(kbState, dt);
    HandleMovement(kbState, dt);

    if (kbState.R) { Teleport(m_SpawnPosition); } // teleport player to starting position
}
