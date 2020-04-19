#include "gametemplate/bot_tiletemplate.h"

namespace bot {

TileTemplate::TileTemplate()
	: GameObjectTemplate()
	, m_texture(nullptr)
	, m_rect(nullptr)
	, m_color(nullptr)
	, m_hp(0)
{
}

} // end of namespace bot