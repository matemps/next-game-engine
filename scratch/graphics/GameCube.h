#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "SimpleMath.h"

#include "Vertex.h"


class GameCube
{
    public:
        template <size_t VertexCount, size_t IndexCount>
        GameCube(Vertex (&vertices)[VertexCount], uint16_t (&indices)[IndexCount]):
            m_Vertices(vertices, vertices + VertexCount),
            m_Indices(indices, indices + IndexCount)
        {}

    public:
        std::vector<Vertex> m_Vertices;
        std::vector<uint16_t> m_Indices;
};
