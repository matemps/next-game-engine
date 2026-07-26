#pragma once

#include "pch.h"
#include "SimpleMath.h"

#include "DeviceResources.h"
#include "Shader.h"
#include "ConstantBuffer.h"

#include "GameRenderer.h"

#include "Block.h"
#include "GameBlock.h"

#include "Plane.h"
#include "GamePlane.h"

#include "WVP.h"


class Renderer
{
    public:
        void Initialize(DX::DeviceResources* deviceResources);
        void BeginFrame();
        void EndFrame();
        void DrawBlock(const GameBlock* gb, WVP* pWVP);
        void DrawPlane(const GamePlane* gp, WVP* pWVP);

        ID3D11Device* GetDevice() const noexcept { return m_deviceResources->GetD3DDevice(); }

    private:
        DX::DeviceResources* m_deviceResources = nullptr;
        Shader m_shader;
        ConstantBuffer m_constantBuffer;
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
};
