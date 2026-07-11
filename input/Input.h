#pragma once

#include <Windows.h>

#include <array>


// Input handler
class Input
{
    public:
        // static functions
        static void Initialize();

        static void SetKeyUp(WPARAM key);
        static void SetKeyDown(WPARAM key);
        static bool IsKeyDown(int key);

    private:
        static std::array<bool, 256> s_Keys;
};