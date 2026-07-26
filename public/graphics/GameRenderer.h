#pragma once

#include <functional>

#include "WVP.h"

#include "GameBlock.h"
#include "GamePlane.h"


class GameRenderer
{
    public:
        inline void SetDrawBlockFn(std::function<void(
            const GameBlock*, WVP*)> callback)
        {
            DrawBlock = callback;
        }

        inline void SetDrawPlaneFn(std::function<void(
            const GamePlane*, WVP*)> callback)
        {
            DrawPlane = callback;
        }

    public:
        std::function<void(const GameBlock*, WVP*)> DrawBlock = nullptr;
        std::function<void(const GamePlane*, WVP*)> DrawPlane = nullptr;
};
