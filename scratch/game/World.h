#pragma once

#include <vector>

#include "SimpleMath.h"
#include "box3d.h"

#include "GameBlock.h"
#include "GamePlane.h"
#include "GameRenderer.h"


using namespace DirectX::SimpleMath;


// Collision categories used to filter box3d queries (e.g. the player's grounded check
// should only care about static world geometry, not its own shape).
constexpr uint64_t c_StaticCollisionCategory = 0x1;
constexpr uint64_t c_PlayerCollisionCategory = 0x2;

constexpr float GRAVITY = 600.0f;

// TO DO: moves these utility functions to a separate header file.

inline b3Vec3 ToB3Vec3(const Vector3& v) { return b3Vec3{ v.x, v.y, v.z }; }
inline b3Quat ToB3Quat(const Quaternion& q) { return b3Quat{ { q.x, q.y, q.z }, q.w }; }
inline Vector3 ToVector3(const b3Vec3& v) { return Vector3(v.x, v.y, v.z); }


struct WorldBlock
{
    GameBlock* Block;
    Matrix Transform;
};

struct WorldPlane
{
    GamePlane* Plane;
    Matrix Transform;
};


class World
{
    public:
        inline World()
        {
            // NextGameEngine uses inch-scale units (e.g. the player is 72 units/inches tall), but box3d
            // assumes 1 unit = 1 meter. If left uncorrected, you get this "sinking" effect while
            // the solver catches up.
            constexpr float c_InchesPerMeter = 39.3701f;
            b3SetLengthUnitsPerMeter(c_InchesPerMeter);

            b3WorldDef worldDef = b3DefaultWorldDef();
            worldDef.gravity = b3Vec3{ 0.0f, -GRAVITY, 0.0f };

            m_PhysicsWorld = b3CreateWorld(&worldDef);
        }

        inline ~World()
        {
            for (WorldBlock& worldBlock : m_Blocks) { delete worldBlock.Block; }
            for (WorldPlane& worldPlane : m_Planes) { delete worldPlane.Plane; }

            b3DestroyWorld(m_PhysicsWorld);
        }

    public:
        inline void AddBlock(
            float length,
            float width,
            float height,
            Matrix transform = Matrix::Identity
        )
        {
            m_Blocks.push_back({ new GameBlock(length, width, height), transform });

            b3BodyId body = CreateStaticBody(transform);
            b3BoxHull hull = b3MakeBoxHull(length, height, width);

            b3ShapeDef shapeDef = b3DefaultShapeDef();
            shapeDef.filter.categoryBits = c_StaticCollisionCategory;

            b3CreateHullShape(body, &shapeDef, &hull.base);
        }

        inline void AddPlane(float length, float width, Matrix transform = Matrix::Identity)
        {
            m_Planes.push_back({ new GamePlane(length, width), transform });

            b3BodyId body = CreateStaticBody(transform);

            // The rendered plane is an infinitely thin quad at local y = 0, so the collision
            // hull is offset downward to give it some solid thickness while keeping its top
            // face flush with the visible surface.
            constexpr float halfThickness = 5.0f;
            b3BoxHull hull = b3MakeOffsetBoxHull(length, halfThickness, width, b3Vec3{ 0.0f, -halfThickness, 0.0f });

            b3ShapeDef shapeDef = b3DefaultShapeDef();
            shapeDef.filter.categoryBits = c_StaticCollisionCategory;

            b3CreateHullShape(body, &shapeDef, &hull.base);
        }

        inline void Render(GameRenderer* renderer, const Matrix view, const Matrix projection) const
        {
            for (const WorldBlock& worldBlock : m_Blocks)
            {
                WVP wvp;
                wvp.World = worldBlock.Transform;
                wvp.View = view;
                wvp.Projection = projection;

                renderer->DrawBlock(worldBlock.Block, &wvp);
            }

            for (const WorldPlane& worldPlane : m_Planes)
            {
                WVP wvp;
                wvp.World = worldPlane.Transform;
                wvp.View = view;
                wvp.Projection = projection;

                renderer->DrawPlane(worldPlane.Plane, &wvp);
            }
        }

    public:
        inline b3WorldId GetPhysicsWorld() const { return m_PhysicsWorld; }

        inline void Step(float dt)
        {
            b3World_Step(m_PhysicsWorld, dt, 4);
        }

    private:
        inline b3BodyId CreateStaticBody(Matrix transform)
        {
            Vector3 scale, translation;
            Quaternion rotation;
            transform.Decompose(scale, rotation, translation);

            b3BodyDef bodyDef = b3DefaultBodyDef();
            bodyDef.type = b3_staticBody;
            bodyDef.position = ToB3Vec3(translation);
            bodyDef.rotation = ToB3Quat(rotation);

            return b3CreateBody(m_PhysicsWorld, &bodyDef);
        }

    private:
        std::vector<WorldBlock> m_Blocks;
        std::vector<WorldPlane> m_Planes;

        b3WorldId m_PhysicsWorld;
};
