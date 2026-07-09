#include "Timer.h"
#include <EASTL/chrono.h>


// constructor
Timer::Timer()
{
    m_LastTime = Clock::now();
}


void Timer::Tick()
{
    auto current = Clock::now();

    m_DeltaTime = eastl::chrono::duration<float>(current - m_LastTime).count();

    m_LastTime = current;
}