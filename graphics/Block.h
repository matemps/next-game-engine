#pragma once

#include "pch.h"
#include "SimpleMath.h"

#include "Vertex.h"
#include "GameBlock.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;


class Block
{
    public:
        void Create(const GameBlock* block, ID3D11Device* device);
        void Draw(ID3D11DeviceContext* context) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

        UINT m_indexCount = 0;
        UINT m_stride = sizeof(Vertex);
        UINT m_offset = 0;
};
