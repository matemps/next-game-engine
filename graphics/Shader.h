#pragma once

#include "pch.h"
#include <string>


class Shader
{
    public:
        void CreateFromFile(ID3D11Device* device, const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath);
        void Bind(ID3D11DeviceContext* context) const;

    public:
        // inline functions
        
        inline ID3D11VertexShader* GetVertexShader() const noexcept { return m_vertexShader.Get(); }
        inline ID3D11PixelShader*  GetPixelShader() const noexcept  { return m_pixelShader.Get(); }
        inline ID3D11InputLayout*  GetInputLayout() const noexcept  { return m_inputLayout.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_pixelShader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_inputLayout;
};
