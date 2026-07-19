#pragma once

#include <functional>

#include "WVP.h"

#include "GameCube.h"


class GameRenderer
{
    public:
        inline void SetDrawCubeFn(std::function<void(
            const GameCube*, WVP*)> callback)
        {
            DrawCube = callback;
        }

    public:
        std::function<void(const GameCube*, WVP*)> DrawCube = nullptr;
};
