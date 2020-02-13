#include "bot_gameobjecttemplate.h"

namespace bot {

GameObjectTemplate::GameObjectTemplate()
    : m_baseComponent(nullptr)
    , m_type(GAMEOBJ_UNKNONWN)
    , m_coverBreathX(0.0f)
    , m_coverBreathY(0.0f)
    , m_hp(0)
    , m_collideBreathX(0.0f)
    , m_collideBreathY(0.0f)
{}

} // end of namespace bot