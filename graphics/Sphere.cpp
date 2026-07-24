#include "pch.h"
#include "Sphere.h"

#include <vector>

using Microsoft::WRL::ComPtr;


void Sphere::Create(const GameSphere* sphere, ID3D11Device* device)
{
    const float radius = sphere->m_Radius;
    const UINT stacks = 16;
    const UINT slices = 24;

    std::vector<Vertex> vertices;
    vertices.reserve((stacks + 1) * (slices + 1));

    for (UINT i = 0; i <= stacks; ++i)
    {
        const float phi = (static_cast<float>(i) / stacks) * XM_PI;

        for (UINT j = 0; j <= slices; ++j)
        {
            const float theta = (static_cast<float>(j) / slices) * XM_2PI;

            Vector3 normal(
                sinf(phi) * cosf(theta),
                cosf(phi),
                sinf(phi) * sinf(theta)
            );

            Vector3 position = normal * radius;
            Color color(normal.x * 0.5f + 0.5f, normal.y * 0.5f + 0.5f, normal.z * 0.5f + 0.5f);

            vertices.push_back({ position, color });
        }
    }

    std::vector<uint16_t> indices;
    indices.reserve(stacks * slices * 6);

    for (UINT i = 0; i < stacks; ++i)
    {
        for (UINT j = 0; j < slices; ++j)
        {
            const uint16_t row0 = static_cast<uint16_t>(i * (slices + 1));
            const uint16_t row1 = static_cast<uint16_t>((i + 1) * (slices + 1));

            const uint16_t a = row0 + static_cast<uint16_t>(j);
            const uint16_t b = row0 + static_cast<uint16_t>(j) + 1;
            const uint16_t c = row1 + static_cast<uint16_t>(j);
            const uint16_t d = row1 + static_cast<uint16_t>(j) + 1;

            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(b);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    m_indexCount = static_cast<UINT>(indices.size());

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Vertex));
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices.data();

    DX::ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf()));

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(uint16_t));
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices.data();

    DX::ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf()));
}

void Sphere::Draw(ID3D11DeviceContext* context) const
{
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    context->DrawIndexed(m_indexCount, 0, 0);
}
