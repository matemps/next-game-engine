#pragma once

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "SimpleMath.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;


struct WVP
{
    Matrix World;
    Matrix View;
    Matrix Projection;
};