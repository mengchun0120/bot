#include "gameobj/bot_goodie_effect.h"

namespace bot {

void GoodieEffect::start()
{
    m_startTime = Clock::now();
}

bool GoodieEffect::expired() const
{
    float duration = getTemplate()->getEffectDuration();

    if (duration == 0.0f)
    {
        return true;
    }
    
    return elapsedTimeS(m_startTime) >= duration;
}

} // end of namespace bot
