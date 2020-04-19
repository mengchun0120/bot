#include "geometry/bot_rectangle.h"
#include "opengl/bot_simpleshaderprogram.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "gametemplate/bot_tiletemplate.h"
#include "gameobj/bot_tile.h"

namespace bot {

Tile::Tile(const TileTemplate* tileTemplate)
	: GameObject(GAME_OBJ_TYPE_TILE)
	, m_tileTemplate(tileTemplate)
{
	m_pos[0] = 0.0f;
	m_pos[1] = 0.0f;
}

Tile::~Tile()
{}

void Tile::present(SimpleShaderProgram& program)
{
	const Rectangle* rect = m_tileTemplate->getRect();
	const Texture* texture = m_tileTemplate->getTexture();
	const Color* color = m_tileTemplate->getColor();

	rect->draw(program, m_pos, nullptr, nullptr, nullptr, texture->textureId(), color);
}

bool Tile::update(float delta, GameScreen& screen)
{
	return true;
}

float Tile::getCoverBreathX() const
{
	return m_tileTemplate->getCoverBreathX();
}

float Tile::getCoverBreathY() const
{
	return m_tileTemplate->getCoverBreathY();
}

float Tile::getCollideBreathX() const
{
	return m_tileTemplate->getCollideBreathX();
}

float Tile::getCollideBreathY() const
{
	return m_tileTemplate->getCollideBreathY();
}

} // end of namespace bot
