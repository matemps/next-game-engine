#pragma once

#include <functional>

#include "WVP.h"

#include "GameCube.h"
#include "GameSphere.h"


class GameRenderer
{
    public:
        inline void SetDrawCubeFn(std::function<void(
            const GameCube*, WVP*)> callback)
        {
            DrawCube = callback;
        }

        inline void SetDrawSphereFn(std::function<void(
            const GameSphere*, WVP*)> callback)
        {
            DrawSphere = callback;
        }

    public:
        std::function<void(const GameCube*, WVP*)> DrawCube = nullptr;
        std::function<void(const GameSphere*, WVP*)> DrawSphere = nullptr;
};
