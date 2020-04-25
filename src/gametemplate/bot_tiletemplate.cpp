#include "gametemplate/bot_tiletemplate.h"

namespace bot {

TileTemplate::TileTemplate()
	: GameObjectTemplate(GAME_OBJ_TYPE_TILE)
	, m_texture(nullptr)
	, m_rect(nullptr)
	, m_color(nullptr)
	, m_hp(0)
{
}

} // end of namespace bot