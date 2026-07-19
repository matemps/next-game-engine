#include "pch.h"
#include "Shader.h"

#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;


namespace
{
    ComPtr<ID3DBlob> CompileShader(const std::wstring& path, const char* entryPoint, const char* target)
    {
        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
        flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ComPtr<ID3DBlob> shaderBlob;
        ComPtr<ID3DBlob> errorBlob;

        HRESULT hr = D3DCompileFromFile(
            path.c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint,
            target,
            flags,
            0,
            shaderBlob.ReleaseAndGetAddressOf(),
            errorBlob.ReleaseAndGetAddressOf()
        );

        if (FAILED(hr))
        {
            if (errorBlob)
            {
                throw std::runtime_error(static_cast<const char*>(errorBlob->GetBufferPointer()));
            }

            DX::ThrowIfFailed(hr);
        }

        return shaderBlob;
    }
}

void Shader::CreateFromFile(ID3D11Device* device, const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath)
{
    ComPtr<ID3DBlob> vertexShaderBlob = CompileShader(vertexShaderPath, "VSMain", "vs_5_0");
    DX::ThrowIfFailed(device->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr,
        m_vertexShader.ReleaseAndGetAddressOf()
    ));

    ComPtr<ID3DBlob> pixelShaderBlob = CompileShader(pixelShaderPath, "PSMain", "ps_5_0");
    DX::ThrowIfFailed(device->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        m_pixelShader.ReleaseAndGetAddressOf()
    ));

    static const D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    DX::ThrowIfFailed(device->CreateInputLayout(
        inputLayoutDesc,
        static_cast<UINT>(std::size(inputLayoutDesc)),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        m_inputLayout.ReleaseAndGetAddressOf()
    ));
}

void Shader::Bind(ID3D11DeviceContext* context) const
{
    context->IASetInputLayout(m_inputLayout.Get());
    context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}
