#pragma once

#include <vector>

#include "SimpleMath.h"

#include "GameBlock.h"
#include "GamePlane.h"
#include "GameRenderer.h"


using namespace DirectX::SimpleMath;


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
        inline ~World()
        {
            for (WorldBlock& worldBlock : m_Blocks) { delete worldBlock.Block; }
            for (WorldPlane& worldPlane : m_Planes) { delete worldPlane.Plane; }
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
        }

        inline void AddPlane(float length, float width, Matrix transform = Matrix::Identity)
        {
            m_Planes.push_back({ new GamePlane(length, width), transform });
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
        
    private:
        std::vector<WorldBlock> m_Blocks;
        std::vector<WorldPlane> m_Planes;
};
