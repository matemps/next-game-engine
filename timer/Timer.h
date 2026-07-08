#pragma once

#include <chrono>   // to do: replace with eastl implementation


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
        using Clock = std::chrono::steady_clock;    // to do: replace with eastl implementation

        Clock::time_point m_LastTime;

        float m_DeltaTime = 0.0f;
};