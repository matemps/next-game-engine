#pragma once

#include "pch.h"
#include "SimpleMath.h"

#include "DeviceResources.h"
#include "Shader.h"
#include "ConstantBuffer.h"

#include "GameRenderer.h"

#include "Cube.h"
#include "GameCube.h"

#include "WVP.h"


class Renderer
{
    public:
        void Initialize(DX::DeviceResources* deviceResources);
        void BeginFrame();
        void EndFrame();
        void DrawCube(const GameCube* gc, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);

        ID3D11Device* GetDevice() const noexcept { return m_deviceResources->GetD3DDevice(); }

    private:
        DX::DeviceResources* m_deviceResources = nullptr;
        Shader m_shader;
        ConstantBuffer<WVP> m_constantBuffer;
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
};
