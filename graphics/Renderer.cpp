#include "pch.h"
#include "Renderer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;


void Renderer::Initialize(DX::DeviceResources* deviceResources)
{
    m_deviceResources = deviceResources;

    auto device = m_deviceResources->GetD3DDevice();

    m_shader.CreateFromFile(device, L"Basic_VS.hlsl", L"Basic_PS.hlsl");

    m_constantBuffer.Create(device);

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.DepthClipEnable = TRUE;

    DX::ThrowIfFailed(device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.ReleaseAndGetAddressOf()));
}

void Renderer::BeginFrame()
{
    auto context = m_deviceResources->GetD3DDeviceContext();

    context->RSSetState(m_rasterizerState.Get());
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_shader.Bind(context);
}

void Renderer::EndFrame()
{
    m_deviceResources->Present();
}

void Renderer::DrawCube(const GameCube* gc, const Matrix& world, const Matrix& view, const Matrix& projection)
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    Cube cube;
    cube.Create(gc, device);

    WVP wvp;
    wvp.World = world.Transpose();
    wvp.View = view.Transpose();
    wvp.Projection = projection.Transpose();

    m_constantBuffer.SetData(context, wvp);

    ID3D11Buffer* buffer = m_constantBuffer.GetBuffer();
    context->VSSetConstantBuffers(0, 1, &buffer);

    cube.Draw(context);
}
