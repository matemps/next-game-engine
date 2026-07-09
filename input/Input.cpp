#include "Input.h"
#include <EASTL/array.h>

eastl::array<bool, 256> Input::s_Keys = {};


void Input::Initialize()
{
    s_Keys.fill(false);
}

void Input::SetKeyDown(WPARAM key)
{
    if (key < s_Keys.size())
        s_Keys[key] = true;
}

void Input::SetKeyUp(WPARAM key)
{
    if (key < s_Keys.size())
        s_Keys[key] = false;
}

bool Input::IsKeyDown(int key)
{
    return s_Keys[key];
}