#ifndef INCLUDE_BOT_PLAYER
#define INCLUDE_BOT_PLAYER

#include "gametemplate/bot_player_template.h"
#include "gameobj/bot_robot.h"

namespace bot {

class Player : public Robot {
public:
	Player(const PlayerTemplate* playerTemplate);

	virtual ~Player();

	const PlayerTemplate* getTemplate() const
	{
		return static_cast<const PlayerTemplate*>(m_template);
	}

	virtual void present(SimpleShaderProgram& program);

	virtual bool update(float delta, GameScreen& screen);
};

} // end of namespace bot

#endif
