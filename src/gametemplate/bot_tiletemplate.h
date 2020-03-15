#ifndef INCLUDE_BOT_TILETEMPLATE
#define INCLUDE_BOT_TILETEMPLATE

#include "gametemplate/bot_gameobjecttemplate.h"

namespace bot {

class Texture;

class TileTemplate: public GameObjectTemplate {
public:
	TileTemplate();

	TileTemplate(const Texture* texture, float coverBreathX, float coverBreathY,
				 float collideBreathX, float collideBreathY,
				 int hp, int flags);

	virtual ~TileTemplate()
	{}

	const Texture* getTexture() const
	{
		return m_texture;
	}

	void setTexture(const Texture* texture)
	{
		m_texture = texture;
	}

protected:
	const Texture* m_texture;
};

} // end of namespace bot

#endif
