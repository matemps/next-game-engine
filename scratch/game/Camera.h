#pragma once

#include <algorithm>
#include <cmath>

#include <DirectXMath.h>
#include "SimpleMath.h"

#include "WVP.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;


class Camera
{
    public:
        Camera() = default;
        inline Camera(Vector3 eye, Vector3 at, Vector3 up = Vector3::UnitY)
            : m_Eye(eye), m_Up(up)
        {
            Vector3 direction = at - eye;
            direction.Normalize();

            m_Pitch = asinf(direction.y);
            m_Yaw = atan2f(direction.x, -direction.z);
        }

    public:
        inline Matrix GetViewMatrix() const
        {
            return Matrix::CreateLookAt(m_Eye, m_Eye + GetForward(), m_Up);
        }

        inline Matrix GetProjectionMatrix() const
        {
            float fovAngleY = m_FovY;

            // This is a simple example of change that can be made when the app is in
            // portrait or snapped view.
            if (m_AspectRatio < 1.0f)
            {
                fovAngleY *= 2.0f;
            }

            return Matrix::CreatePerspectiveFieldOfView(fovAngleY, m_AspectRatio, m_NearZ, m_FarZ);
        }

        // Assembles a WVP for the given world transform using this camera's view/projection.
        inline WVP GetWVP(Matrix world) const
        {
            WVP wvp;
            wvp.World = world;
            wvp.View = GetViewMatrix();
            wvp.Projection = GetProjectionMatrix();
            return wvp;
        }

    public:
        // movement (amount is world units, already scaled by speed/dt)
        inline void MoveForward(float amount) { m_Eye += GetForward() * amount; }
        inline void MoveRight(float amount) { m_Eye += GetRight() * amount; }
        inline void MoveUp(float amount) { m_Eye += Vector3::UnitY * amount; }

        // look (deltas in radians, already scaled by speed/dt)
        inline void Rotate(float yawDelta, float pitchDelta)
        {
            m_Yaw += yawDelta;
            m_Pitch += pitchDelta;

            // Avoid gimbal flip by keeping pitch just shy of straight up/down.
            const float pitchLimit = XM_PIDIV2 - 0.01f;
            m_Pitch = std::clamp(m_Pitch, -pitchLimit, pitchLimit);
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
        // getters
        inline Vector3 GetPosition() const { return m_Eye; }
        inline Vector3 GetUp() const { return m_Up; }

        // setters
        inline void SetPosition(Vector3 eye) { m_Eye = eye; }
        inline void SetUp(Vector3 up) { m_Up = up; }
        inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
        inline void SetLens(float fovAngleY, float nearZ, float farZ)
        {
            m_FovY = fovAngleY;
            m_NearZ = nearZ;
            m_FarZ = farZ;
        }

    private:
        Vector3 m_Eye = Vector3(0.0f, 0.7f, 1.5f);
        Vector3 m_Up = Vector3::UnitY;

        float m_Yaw = 0.0f;
        float m_Pitch = 0.0f;

        float m_FovY = 70.0f * XM_PI / 180.0f;
        float m_NearZ = 0.01f;
        float m_FarZ = 100.0f;
        float m_AspectRatio = 1.0f;
};
