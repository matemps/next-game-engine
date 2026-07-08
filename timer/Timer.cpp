#include "Timer.h"


// constructor
Timer::Timer()
{
    m_LastTime = Clock::now();
}


void Timer::Tick()
{
    auto current = Clock::now();

    // to do: replace with eastl implementation
    m_DeltaTime = std::chrono::duration<float>(current - m_LastTime).count();

    m_LastTime = current;
}