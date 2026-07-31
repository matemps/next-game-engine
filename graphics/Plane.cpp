#include "pch.h"
#include "Plane.h"

using Microsoft::WRL::ComPtr;


void ::Plane::Create(const GamePlane* plane, ID3D11Device* device)
{
    const float l = plane->m_Length / 2.0f;
    const float w = plane->m_Width / 2.0f;

    Vertex vertices[] =
    {
        { Vector3(-l, 0.0f, -w), Color(1.0f, 1.0f, 1.0f) },
        { Vector3(-l, 0.0f,  w), Color(1.0f, 1.0f, 1.0f) },
        { Vector3( l, 0.0f, -w), Color(1.0f, 1.0f, 1.0f) },
        { Vector3( l, 0.0f,  w), Color(1.0f, 1.0f, 1.0f) },
    };

    uint16_t indices[] =
    {
        0, 1, 2,
        2, 1, 3,
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

void ::Plane::Draw(ID3D11DeviceContext* context) const
{
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    context->DrawIndexed(m_indexCount, 0, 0);
}
