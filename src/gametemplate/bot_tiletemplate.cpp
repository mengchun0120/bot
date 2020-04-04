#include "gametemplate/bot_tiletemplate.h"

namespace bot {

TileTemplate::TileTemplate()
	: GameObjectTemplate()
	, m_texture(nullptr)
	, m_rect(nullptr)
	, m_color(nullptr)
{
}

TileTemplate::TileTemplate(const Texture* texture, const Rectangle* rect, const Color* color,
	                       float coverBreathX, float coverBreathY,
	                       float collideBreathX, float collideBreathY,
	                       int hp, int flags)
	: GameObjectTemplate(coverBreathX, coverBreathY, collideBreathX, collideBreathY, hp, flags)
	, m_texture(texture)
	, m_rect(rect)
	, m_color(color)
{
}

} // end of namespace bot