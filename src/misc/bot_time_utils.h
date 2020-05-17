#ifndef INCLUDE_BOT_TIME_UTILS
#define INCLUDE_BOT_TIME_UTILS

#include <chrono>

namespace bot {

typedef std::chrono::high_resolution_clock Clock;
typedef Clock::time_point TimePoint;

inline float timeDistMs(const TimePoint& startTime, const TimePoint& endTime)
{
    using namespace std::chrono;
    milliseconds dur = duration_cast<milliseconds>(endTime - startTime);
    return static_cast<float>(dur.count());
}

inline float elapsedTimeMs(const TimePoint& startTime)
{
    return timeDistMs(startTime, Clock::now());
}

} // end of namespace bot

#endif
