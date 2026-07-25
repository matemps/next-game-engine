#pragma once

#include <functional>

#include "WVP.h"

#include "GameCube.h"
#include "GamePlane.h"


class GameRenderer
{
    public:
        inline void SetDrawCubeFn(std::function<void(
            const GameCube*, WVP*)> callback)
        {
            DrawCube = callback;
        }

        inline void SetDrawPlaneFn(std::function<void(
            const GamePlane*, WVP*)> callback)
        {
            DrawPlane = callback;
        }

    public:
        std::function<void(const GameCube*, WVP*)> DrawCube = nullptr;
        std::function<void(const GamePlane*, WVP*)> DrawPlane = nullptr;
};
