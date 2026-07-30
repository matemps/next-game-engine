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
        void ApplyFriction(Vector3& velocity, float dt);
        void HandleMovement(Keyboard::State kbState, float dt);
        float CalculateJumpHeight();
        void Jump();
        void Crouch();
        bool IsGrounded();
        void SyncCameraToBody();

    private:
        enum PlayerState
        {
            Crouching,
            Standing,
            Walking
        };
    
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
        static constexpr float EYE_LEVEL = 64.0f;
        static constexpr float EYE_LEVEL_CROUCH = 28.0f;

        // movement speed
        static constexpr float MOVEMENT_SPEED_WALK = 150.0f;
        static constexpr float MOVEMENT_SPEED_CROUCH = 63.3f;
        static constexpr float MOVEMENT_SPEED_MAX = 320.0f;

        // jump height

        // crouching
        static constexpr float CROUCHING_JUMP_HEIGHT = 21.0f;
        
        // standing
        static constexpr float STANDING_JUMP_HEIGHT = 20.0f;
        static constexpr float STANDING_JUMP_CROUCH_HEIGHT = 56.0f;

        // walking
        static constexpr float WALKING_JUMP_HEIGHT = 20.0f;
        static constexpr float WALKING_JUMP_CROUCH_HEIGHT = 56.0f;

        // other
        static constexpr float FRICTION = 4.0f;
        static constexpr float STOP_SPEED = 100.0f;
        static constexpr float ACCELERATE = 10.0f;

    private:
        b3WorldId m_PhysicsWorld;
        b3BodyId m_Body;
        b3ShapeId m_Shape;

        Camera* m_PlayerCamera = new Camera();

        PlayerState m_PlayerState = Standing;

        float m_EyeLevel = EYE_LEVEL;

        float m_WishSpeed = MOVEMENT_SPEED_WALK;

        bool m_IsCrouched = false;
};
