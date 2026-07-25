#pragma once

#include "pch.h"
#include "WVP.h"

class ConstantBuffer
{
    public:
        void Create(ID3D11Device* device)
        {
            D3D11_BUFFER_DESC desc = {};
            desc.ByteWidth = sizeof(WVP);
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

            DX::ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_buffer.ReleaseAndGetAddressOf()));
        }

        void SetData(ID3D11DeviceContext* context, const WVP& data)
        {
            context->UpdateSubresource(m_buffer.Get(), 0, nullptr, &data, 0, 0);
        }

        ID3D11Buffer* GetBuffer() const noexcept { return m_buffer.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
};
