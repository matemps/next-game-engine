#include "pch.h"
#include "Cube.h"

using Microsoft::WRL::ComPtr;


void Cube::Create(const GameCube* cube, ID3D11Device* device)
{
    m_indexCount = static_cast<UINT>(cube->m_Indices.size());

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = static_cast<UINT>(cube->m_Vertices.size() * sizeof(Vertex));
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = cube->m_Vertices.data();

    DX::ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf()));

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.ByteWidth = static_cast<UINT>(cube->m_Indices.size() * sizeof(uint16_t));
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = cube->m_Indices.data();

    DX::ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf()));
}

void Cube::Draw(ID3D11DeviceContext* context) const
{
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    context->DrawIndexed(m_indexCount, 0, 0);
}
