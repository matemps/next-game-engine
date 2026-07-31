#include "pch.h"
#include "Block.h"

using Microsoft::WRL::ComPtr;


void Block::Create(const GameBlock* block, ID3D11Device* device)
{
    const float l = block->m_Length / 2.0f;
    const float w = block->m_Width / 2.0f;
    const float h = block->m_Height / 2.0f;

    Vertex vertices[] =
    {
        { Vector3(-l, -h, -w), Color(0.0f, 0.0f, 0.0f) },
        { Vector3(-l, -h,  w), Color(0.0f, 0.0f, 1.0f) },
        { Vector3(-l,  h, -w), Color(0.0f, 1.0f, 0.0f) },
        { Vector3(-l,  h,  w), Color(0.0f, 1.0f, 1.0f) },
        { Vector3( l, -h, -w), Color(1.0f, 0.0f, 0.0f) },
        { Vector3( l, -h,  w), Color(1.0f, 0.0f, 1.0f) },
        { Vector3( l,  h, -w), Color(1.0f, 1.0f, 0.0f) },
        { Vector3( l,  h,  w), Color(1.0f, 1.0f, 1.0f) },
    };

    uint16_t indices[] =
    {
        0,2,1, 1,2,3,
        4,5,6, 5,7,6,
        0,1,5, 0,5,4,
        2,6,7, 2,7,3,
        0,4,6, 0,6,2,
        1,3,7, 1,7,5,
    };

    m_indexCount = static_cast<UINT>(std::size(indices));

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(vertices));
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices;

    DX::ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf()));

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(indices));
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices;

    DX::ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf()));
}

void Block::Draw(ID3D11DeviceContext* context) const
{
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    context->DrawIndexed(m_indexCount, 0, 0);
}
