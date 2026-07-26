#pragma once


class GameBlock
{
    public:
        inline GameBlock(float length, float width, float height): 
            m_Length(length),
            m_Width(width),
            m_Height(height)
        {}

    public:
        float m_Length;
        float m_Width;
        float m_Height;
};
