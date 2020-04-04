#include "misc/bot_config.h"
#include "gameutil/bot_timedeltasmoother.h"

namespace bot {

TimeDeltaSmoother::TimeDeltaSmoother()
    : m_sumTimeDelta(0.0f)
    , m_totalSlots(0)
    , m_lastSlot(0)
{
}


TimeDeltaSmoother::~TimeDeltaSmoother()
{
}

void TimeDeltaSmoother::init(int timeDeltaHistoryLen)
{
    m_timeDeltaHistory.resize(timeDeltaHistoryLen);
}

void TimeDeltaSmoother::start()
{
    using namespace std::chrono;

    m_prevTime = high_resolution_clock::now();

    unsigned int histSize = m_timeDeltaHistory.size();
    for (unsigned int i = 0; i < histSize; ++i) 
    {
        m_timeDeltaHistory[i] = 0.0f;
    }

    m_sumTimeDelta = 0.0f;
    m_totalSlots = 0;
    m_lastSlot = 0;
}

float TimeDeltaSmoother::getTimeDelta()
{
    using namespace std::chrono;

    TimePoint curTime = high_resolution_clock::now();
    duration<float> dur = curTime - m_prevTime;
    float timeDelta = dur.count();

    unsigned int histSize = m_timeDeltaHistory.size();

    if (m_totalSlots == m_timeDeltaHistory.size()) 
    {
        m_sumTimeDelta += timeDelta - m_timeDeltaHistory[m_lastSlot];
    } 
    else
    {
        m_sumTimeDelta += timeDelta;
        m_totalSlots++;
    }

    m_timeDeltaHistory[m_lastSlot] = timeDelta;
    m_lastSlot = (m_lastSlot + 1) % histSize;
    m_prevTime = curTime;

    return m_sumTimeDelta / m_totalSlots;
}

} // end of namespace bot

