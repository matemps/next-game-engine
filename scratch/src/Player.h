#pragma once

#include <cmath>

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include "Keyboard.h"

#include "Camera.h"
#include "World.h"


using namespace DirectX;


// TO DO: Implement Hl2 player physics


class Player
{
    public:
        Player(b3WorldId physicsWorld, Vector3 spawnPosition);
        ~Player();

    public:
        void Update(Keyboard::State kbState, float dt);

    public:
        // getters

        inline Camera* GetPlayerCamera() { return m_PlayerCamera; }

    private:
        void Crouch();
        bool IsGrounded();
        void SyncCameraToBody();

    private:
        // collision hull dimensions
        static constexpr float c_Width = 32.0f;
        static constexpr float c_Depth = 32.0f;
        static constexpr float c_Height = 72.0f;

        // crouched collision hull dimensions
        static constexpr float c_CrouchWidth = 32.0f;
        static constexpr float c_CrouchDepth = 32.0f;
        static constexpr float c_CrouchHeight = 36.0f;

        // half-extent
        static constexpr float c_HalfWidth = c_Width * 0.5f;
        static constexpr float c_HalfDepth = c_Depth * 0.5f;
        static constexpr float c_HalfHeight = c_Height * 0.5f;

        static constexpr float c_CrouchHalfWidth = c_CrouchWidth * 0.5f;
        static constexpr float c_CrouchHalfDepth = c_CrouchDepth * 0.5f;
        static constexpr float c_CrouchHalfHeight = c_CrouchHeight * 0.5f;

    private:
        b3WorldId m_PhysicsWorld;
        b3BodyId m_Body;
        b3ShapeId m_Shape;

        Vector3 m_EyeLevel = Vector3(0, 64, 0);
        Camera* m_PlayerCamera = new Camera();

        float m_MovementSpeed = 150.0f;
        float m_JumpSpeed = 500.0f;

        bool m_IsCrouched = false;
};
