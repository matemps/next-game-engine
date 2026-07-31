#pragma once

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
        void RotateCamera(Keyboard::State kbState, float dt);
        bool IsGrounded();
        void ApplyFriction(Vector3& velocity, float dt);
        void Jump(Vector3& velocity);
        void SyncCameraToBody();
        void HandleMovement(Keyboard::State kbState, float dt);
        void Teleport(Vector3 position);

    private:
        // collision hull dimensions
        static constexpr float HULL_WIDTH = 32.0f;
        static constexpr float HULL_DEPTH = 32.0f;
        static constexpr float HULL_HEIGHT = 56.0f;
        static constexpr float HALF_WIDTH = HULL_WIDTH * 0.5f;
        static constexpr float HALF_DEPTH = HULL_DEPTH * 0.5f;
        static constexpr float HALF_HEIGHT = HULL_HEIGHT * 0.5f;

        // eye level
        static constexpr float EYE_LEVEL = 46.0f;

        // movement speed cap
        static constexpr float MAX_SPEED = 320.0f;

        // per-direction input speeds
        static constexpr float FORWARD_SPEED = 200.0f;
        static constexpr float BACK_SPEED = 200.0f;
        static constexpr float SIDE_SPEED = 350.0f;

        // jump velocity
        static constexpr float JUMP_VELOCITY = 270.0f;

        // ground
        static constexpr float FRICTION = 4.0f;
        static constexpr float STOP_SPEED = 100.0f;
        static constexpr float GROUND_CHECK_DISTANCE = 2.0f;
        static constexpr float GROUND_ACCELERATE = 10.0f;

        // air control
        static constexpr float AIR_ACCELERATE = 10.0f;
        static constexpr float AIR_SPEED_CAP = 30.0f;

        // velocity clamp
        static constexpr float MAX_VELOCITY = 2000.0f;

    private:
        b3WorldId m_PhysicsWorld;
        b3BodyId m_Body;
        b3ShapeId m_Shape;

        Vector3 m_SpawnPosition;

        Camera* m_PlayerCamera = new Camera();

        bool m_JumpHeld = false;
};
