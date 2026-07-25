#pragma once


class GamePlane
{
    public:
        inline GamePlane(float width, float depth) : m_Width(width), m_Depth(depth) {}

    public:
        float m_Width;
        float m_Depth;
};
