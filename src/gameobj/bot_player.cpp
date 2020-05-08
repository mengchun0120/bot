#include "misc/bot_log.h"
#include "gameobj/bot_player.h"

namespace bot {

Player::Player(const PlayerTemplate* playerTemplate)
	: Robot(static_cast<const RobotTemplate*>(playerTemplate))
{

}

Player::~Player()
{

}

void Player::present(SimpleShaderProgram& program)
{
	Robot::present(program);
}

bool Player::update(float delta, GameScreen& screen)
{
	if (!Robot::update(delta, screen))
	{
		return false;
	}

	return true;
}

} // end of namespace bot
