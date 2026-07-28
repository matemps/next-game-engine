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

    b3BoxHull hull = b3MakeBoxHull(c_HalfWidth, c_HalfHeight, c_HalfDepth);

    b3ShapeDef shapeDef = b3DefaultShapeDef();
    shapeDef.filter.categoryBits = c_PlayerCollisionCategory;

    m_Shape = b3CreateHullShape(m_Body, &shapeDef, &hull.base);

    SyncCameraToBody();
}

Player::~Player()
{
    delete m_PlayerCamera;
}

void Player::Crouch()
{
    m_IsCrouched = !m_IsCrouched;

    m_EyeLevel = m_IsCrouched ? Vector3(0, 28, 0) : Vector3(0, 64, 0);
    m_MovementSpeed = m_IsCrouched ? 63.3f : 150.0f;

    b3BoxHull hull = m_IsCrouched
        ? b3MakeBoxHull(c_CrouchHalfWidth, c_CrouchHalfHeight, c_CrouchHalfDepth)
        : b3MakeBoxHull(c_HalfWidth, c_HalfHeight, c_HalfDepth);

    b3Shape_SetHull(m_Shape, &hull.base);
}

bool Player::IsGrounded()
{
    b3QueryFilter filter = b3DefaultQueryFilter();
    filter.maskBits = c_StaticCollisionCategory;

    // Cast a short ray from the body center down past the feet - if it hits static
    // world geometry the player is standing on something and may jump.
    b3RayResult result = b3World_CastRayClosest(
        m_PhysicsWorld,
        b3Body_GetPosition(m_Body),
        b3Vec3{ 0.0f, -(c_HalfHeight + 2.0f), 0.0f },
        filter
    );

    return result.hit;
}

void Player::SyncCameraToBody()
{
    float halfHeight = m_IsCrouched ? c_CrouchHalfHeight : c_HalfHeight;

    Vector3 center = ToVector3(b3Body_GetPosition(m_Body));
    Vector3 feet = center - Vector3(0.0f, halfHeight, 0.0f);

    m_PlayerCamera->SetPosition(feet + m_EyeLevel);
}

void Player::Update(Keyboard::State kbState, float dt)
{
    if (kbState.Left) { m_PlayerCamera->RotateLeft(dt); }
    if (kbState.Right) { m_PlayerCamera->RotateRight(dt); }
    if (kbState.Up) { m_PlayerCamera->RotateUp(dt); }
    if (kbState.Down) { m_PlayerCamera->RotateDown(dt); }

    if ((kbState.LeftControl && !m_IsCrouched) 
        || (!kbState.LeftControl && m_IsCrouched)) { Crouch(); }

    Vector3 forward = m_PlayerCamera->GetForward();
    forward.y = 0.0f;
    forward.Normalize();

    Vector3 right = m_PlayerCamera->GetRight();

    Vector3 moveDirection = Vector3::Zero;
    if (kbState.W) { moveDirection += forward; }
    if (kbState.S) { moveDirection -= forward; }
    if (kbState.D) { moveDirection += right; }
    if (kbState.A) { moveDirection -= right; }

    if (moveDirection != Vector3::Zero) { moveDirection.Normalize(); }

    Vector3 velocity = moveDirection * m_MovementSpeed;
    velocity.y = ToVector3(b3Body_GetLinearVelocity(m_Body)).y;

    if (kbState.Space && IsGrounded()) { velocity.y = m_JumpSpeed; }

    b3Body_SetLinearVelocity(m_Body, ToB3Vec3(velocity));

    SyncCameraToBody();
}
