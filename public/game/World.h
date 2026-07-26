#pragma once

#include <vector>

#include "SimpleMath.h"

#include "GameCube.h"
#include "GamePlane.h"
#include "GameRenderer.h"

#include "Camera.h"


using namespace DirectX::SimpleMath;


struct WorldCube
{
    GameCube* Cube;
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
        inline ~World()
        {
            for (WorldCube& worldCube : m_Cubes) { delete worldCube.Cube; }
            for (WorldPlane& worldPlane : m_Planes) { delete worldPlane.Plane; }
        }

    public:
        inline void AddCube(float size, Matrix transform = Matrix::Identity)
        {
            m_Cubes.push_back({ new GameCube(size), transform });
        }

        inline void AddPlane(float width, float depth, Matrix transform = Matrix::Identity)
        {
            m_Planes.push_back({ new GamePlane(width, depth), transform });
        }

        inline void Render(GameRenderer* renderer, const Camera* camera) const
        {
            for (const WorldCube& worldCube : m_Cubes)
            {
                WVP wvp = camera->GetWVP(worldCube.Transform);
                renderer->DrawCube(worldCube.Cube, &wvp);
            }

            for (const WorldPlane& worldPlane : m_Planes)
            {
                WVP wvp = camera->GetWVP(worldPlane.Transform);
                renderer->DrawPlane(worldPlane.Plane, &wvp);
            }
        }

    public:
        // getters
        
        inline const std::vector<WorldCube>& GetCubes() const { return m_Cubes; }
        inline const std::vector<WorldPlane>& GetPlanes() const { return m_Planes; }

    private:
        std::vector<WorldCube> m_Cubes;
        std::vector<WorldPlane> m_Planes;
};
