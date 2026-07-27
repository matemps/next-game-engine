#pragma once

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include "Keyboard.h"

#include "Camera.h"


using namespace DirectX;


class Player
{
    public:
        void Update(Keyboard::State kbState, float dt);

    public:
        void Crouch();

    public:
        // getters

        inline Camera* GetPlayerCamera() { return m_PlayerCamera; }

    private:
        Vector3 m_EyeLevel = Vector3(0, 64, 0);
        Camera* m_PlayerCamera = new Camera();

        float m_MovementSpeed = 150.0f;

        bool m_IsCrouched = false;
};