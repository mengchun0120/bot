#include "gametemplate/bot_missiletemplate.h"

namespace bot {

MissileTemplate::MissileTemplate()
	: GameObjectTemplate()
	, m_speed(0.0f)
	, m_explosionPower(0)
	, m_explosionBreath(0.0f)
	, m_texture(nullptr)
	, m_rect(nullptr)
	, m_explosionTemplate(nullptr)
{

}

MissileTemplate::MissileTemplate(float coverBreathX, float coverBreathY, float collideBreathX, float collideBreathY,
	                             int hp, float flags, float speed, int explosionPower, float explosionBreath,
								 const Texture* texture, const Rectangle* rect,
	                             const AnimationTemplate* explosionTemplate)
	: GameObjectTemplate(coverBreathX, coverBreathY, collideBreathX, collideBreathY, hp, flags)
	, m_speed(speed)
	, m_explosionPower(explosionPower)
	, m_explosionBreath(explosionBreath)
	, m_texture(texture)
	, m_rect(rect)
	, m_explosionTemplate(explosionTemplate)
{
}

MissileTemplate::~MissileTemplate()
{
}

} // end of namespace bot
