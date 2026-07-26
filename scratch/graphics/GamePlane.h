#pragma once


class GamePlane
{
    public:
        inline GamePlane(float length, float width): 
            m_Length(length), m_Width(width) {}

    public:
        float m_Length;
        float m_Width;
};
