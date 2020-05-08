#include "gametemplate/bot_game_object_template.h"

namespace bot {

GameObjectTemplate::GameObjectTemplate(GameObjectType type)
    : m_type(type)
    , m_coverBreathX(0.0f)
    , m_coverBreathY(0.0f)
    , m_collideBreathX(0.0f)
    , m_collideBreathY(0.0f)
    , m_flags(0)
{}

GameObjectTemplate::GameObjectTemplate(GameObjectType type, float coverBreathX, float coverBreathY,
                                       float collideBreathX, float collideBreathY, int flags)
    : m_type(type)
    , m_coverBreathX(coverBreathX)
    , m_coverBreathY(coverBreathY)
    , m_collideBreathX(collideBreathX)
    , m_collideBreathY(collideBreathY)
    , m_flags(flags)
{

}

} // end of namespace bot