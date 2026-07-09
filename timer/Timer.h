#pragma once

#include <EASTL/chrono.h>


class Timer
{
    public:
        // constructor
        Timer();
        
    public:
        // member functions
        void Tick();

    public:
        // inline functions
        inline float GetDeltaTime() const { return m_DeltaTime; }

    private:
        using Clock = eastl::chrono::steady_clock;

        Clock::time_point m_LastTime;

        float m_DeltaTime = 0.0f;
};