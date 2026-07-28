#pragma once

#include <cmath>

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include "Keyboard.h"

#include "Camera.h"
#include "World.h"


using namespace DirectX;


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
        static constexpr float HULL_WIDTH = 32.0f;
        static constexpr float HULL_DEPTH = 32.0f;
        static constexpr float HULL_HEIGHT = 72.0f;
        static constexpr float HULL_HEIGHT_CROUCH = 37.0f;
        static constexpr float HALF_WIDTH = HULL_WIDTH * 0.5f;
        static constexpr float HALF_DEPTH = HULL_DEPTH * 0.5f;
        static constexpr float HALF_HEIGHT = HULL_HEIGHT * 0.5f;
        static constexpr float HALF_HEIGHT_CROUCH = HULL_HEIGHT_CROUCH * 0.5f;

        // eye position
        static constexpr int EYE_LEVEL = 64;
        static constexpr int EYE_LEVEL_CROUCH = 28;

        // movement speed
        static constexpr float MOVEMENT_SPEED = 150.0f;
        static constexpr float MOVEMENT_SPEED_CROUCH = 63.3f;

    private:
        b3WorldId m_PhysicsWorld;
        b3BodyId m_Body;
        b3ShapeId m_Shape;

        Camera* m_PlayerCamera = new Camera();
        
        int m_EyeLevel = EYE_LEVEL;

        float m_MovementSpeed = 150.0f;

        bool m_IsCrouched = false;
};
