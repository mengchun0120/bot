#include "gametemplate/bot_gameobjecttemplate.h"

namespace bot {

GameObjectTemplate::GameObjectTemplate()
    : m_coverBreathX(0.0f)
    , m_coverBreathY(0.0f)
    , m_collideBreathX(0.0f)
    , m_collideBreathY(0.0f)
    , m_hp(0) 
    , m_flags(0)
{}

GameObjectTemplate::GameObjectTemplate(float coverBreathX, float coverBreathY,
                                       float collideBreathX, float collideBreathY,
                                       int hp, int flags)
    : m_coverBreathX(coverBreathX)
    , m_coverBreathY(coverBreathY)
    , m_collideBreathX(collideBreathX)
    , m_collideBreathY(collideBreathY)
    , m_hp(hp)
    , m_flags(flags)
{

}

} // end of namespace bot