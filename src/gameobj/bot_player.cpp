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

void Player::present(ShaderProgram& program)
{
	Robot::present(program);
}

void Player::update(float delta, GameScreen& screen)
{
    updateMoveAbility(delta, screen);

    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return;
    }

    updateShootAbility(screen);
}

} // end of namespace bot
