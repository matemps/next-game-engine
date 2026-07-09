#pragma once

#include <Windows.h>

#include <EASTL/array.h>


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
        static eastl::array<bool, 256> s_Keys;
};