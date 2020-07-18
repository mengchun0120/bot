#ifndef INCLUDE_BOT_TILE_TEMPLATE
#define INCLUDE_BOT_TILE_TEMPLATE

#include <rapidjson/document.h>
#include "gametemplate/bot_game_object_template.h"

namespace bot {

class Texture;
class Rectangle;
class Color;

class TileTemplate: public GameObjectTemplate {
public:
    static TileTemplate* create(const rapidjson::Value& elem);
    
    TileTemplate();

	virtual ~TileTemplate()
	{}

    bool init(const rapidjson::Value& elem);

	const Texture* getTexture() const
	{
		return m_texture;
	}

	void setTexture(const Texture* texture)
	{
		m_texture = texture;
	}

	const Rectangle* getRect() const
	{
		return m_rect;
	}

	void setRect(const Rectangle* rect)
	{
		m_rect = rect;
	}

	const Color* getColor() const
	{
		return m_color;
	}

	void setColor(const Color* color)
	{
		m_color = color;
	}

	int getHP() const
	{
		return m_hp;
	}

	void setHP(int hp)
	{
		m_hp = hp;
	}

protected:
	const Texture* m_texture;
	const Rectangle* m_rect;
	const Color* m_color;
	int m_hp;
};

} // end of namespace bot

#endif
