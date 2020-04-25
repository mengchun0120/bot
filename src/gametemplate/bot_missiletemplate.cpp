#include "gametemplate/bot_missiletemplate.h"

namespace bot {

MissileTemplate::MissileTemplate()
	: GameObjectTemplate(GAME_OBJ_TYPE_MISSILE)
	, m_speed(0.0f)
	, m_explosionPower(0)
	, m_explosionBreath(0.0f)
	, m_texture(nullptr)
	, m_rect(nullptr)
	, m_color(nullptr)
	, m_explosionTemplate(nullptr)
{

}

} // end of namespace bot
