#include "Player.h"


void Player::Crouch()
{
    m_IsCrouched = !m_IsCrouched;

    m_EyeLevel = m_IsCrouched ? Vector3(0, 28, 0) : Vector3(0, 64, 0);
    m_MovementSpeed = m_IsCrouched ? 63.3f : 150.0f;
}

void Player::Update(Keyboard::State kbState, float dt)
{
    // TO DO: handle input in separate function

    if (kbState.W) { /* move forward */ }
    if (kbState.S) { /* move backward */ }
    if (kbState.A) { /* move left */ }
    if (kbState.D) { /* move right */ }

    if (kbState.Space) { /* jump */ }
    if (kbState.LeftControl) { /* crouch */ }

    if (kbState.Left) { m_PlayerCamera->RotateLeft(dt); }
    if (kbState.Right) { m_PlayerCamera->RotateRight(dt); }
    if (kbState.Up) { m_PlayerCamera->RotateUp(dt); }
    if (kbState.Down) { m_PlayerCamera->RotateDown(dt); }
}