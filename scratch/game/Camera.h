#pragma once

#include <algorithm>
#include <cmath>

#include <DirectXMath.h>
#include "SimpleMath.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;


class Camera
{
    public:
        inline Matrix GetViewMatrix() const
        {
            return Matrix::CreateLookAt(
                m_Eye,
                m_Eye + GetForward(),
                m_Up
            );
        }

        inline Matrix GetProjectionMatrix() const
        {
            return Matrix::CreatePerspectiveFieldOfView(
                m_Fov, m_AspectRatio,
                m_NearPlane,
                m_FarPlane
            );
        }

    public:
        inline Vector3 GetForward() const
        {
            float cosPitch = cosf(m_Pitch);
            return Vector3(cosPitch * sinf(m_Yaw), sinf(m_Pitch), -cosPitch * cosf(m_Yaw));
        }

        inline Vector3 GetRight() const
        {
            Vector3 right = GetForward().Cross(Vector3::UnitY);
            right.Normalize();
            return right;
        }

    public:
        // movement

        inline void MoveForward(float dt)
        {
            float moveAmount = m_MoveSpeed * dt;
            m_Eye += GetForward() * moveAmount;
        }

        inline void MoveBackward(float dt)
        {
            float moveAmount = m_MoveSpeed * dt;
            m_Eye -= GetForward() * moveAmount;
        }

        inline void MoveRight(float dt)
        {
            float moveAmount = m_MoveSpeed * dt;
            m_Eye += GetRight() * moveAmount;
        }

        inline void MoveLeft(float dt)
        {
            float moveAmount = m_MoveSpeed * dt;
            m_Eye -= GetRight() * moveAmount;
        }

        inline void MoveUp(float dt)
        {
            float moveAmount = m_MoveSpeed * dt;
            m_Eye += Vector3::UnitY * moveAmount;
        }

        inline void MoveDown(float dt)
        {
            float moveAmount = m_MoveSpeed * dt;
            m_Eye -= Vector3::UnitY * moveAmount;
        }

    public:
        // rotation
        
        inline void RotateRight(float dt)
        {
            float rotateAmount = m_RotateSpeed * dt;
            m_Yaw += rotateAmount;
        }

        inline void RotateLeft(float dt)
        {
            float rotateAmount = m_RotateSpeed * dt;
            m_Yaw -= rotateAmount;
        }

        inline void RotateUp(float dt)
        {
            float rotateAmount = m_RotateSpeed * dt;

            // Avoid gimbal flip by keeping pitch just shy of straight up/down.
            const float pitchLimit = XM_PIDIV2 - 0.01f;
            m_Pitch = std::clamp(
                m_Pitch + rotateAmount,
                -pitchLimit,
                pitchLimit
            );
        }

        inline void RotateDown(float dt)
        {
            float rotateAmount = m_RotateSpeed * dt;

            // Avoid gimbal flip by keeping pitch just shy of straight up/down.
            const float pitchLimit = XM_PIDIV2 - 0.01f;
            m_Pitch = std::clamp(
                m_Pitch - rotateAmount,
                -pitchLimit,
                pitchLimit
            );
        }

    public:
        // setters

        inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

    private:
        // eye level (default: 64 units)
        float m_EyeLevel = 64.0f;

        // camera movement speed (default: 150 units per second)
        float m_MoveSpeed = 150.0f;

        // camera rotation speed (default: 90 degrees per second)
        float m_RotateSpeed = XM_PI / 2.0f;

        // fov (default: 75 degrees)
        float m_Fov = 75.0f * (XM_PI / 180.0f);

        // yaw & pitch
        float m_Yaw = 0.0f;
        float m_Pitch = 0.0f;

        // aspect ratio
        float m_AspectRatio = 1.0f;

        // near & far plane
        float m_NearPlane = 7.0f;
        float m_FarPlane = 28377.92f;

    private:
        // 3d vectors

        Vector3 m_Eye = Vector3(0.0f, m_EyeLevel, 0.0f);
        Vector3 m_Up = Vector3::UnitY;
};