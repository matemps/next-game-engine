#pragma once

#include <functional>

#include "SimpleMath.h"

#include "GameCube.h"


class GameRenderer
{
    public:
        inline void SetDrawCubeFn(std::function<void(
            const GameCube*,
            DirectX::SimpleMath::Matrix,
            DirectX::SimpleMath::Matrix,
            DirectX::SimpleMath::Matrix)>
        callback)
        {
            DrawCube = callback;
        }

    public:
        std::function<void(
            const GameCube*,
            DirectX::SimpleMath::Matrix,
            DirectX::SimpleMath::Matrix,
            DirectX::SimpleMath::Matrix)> DrawCube = nullptr;
};
