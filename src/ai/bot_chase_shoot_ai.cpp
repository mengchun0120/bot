#include "misc/bot_log.h"
#include "misc/bot_time_utils.h"
#include "misc/bot_math_utils.h"
#include "gameobj/bot_robot.h"
#include "gameobj/bot_player.h"
#include "screen/bot_game_screen.h"
#include "ai/bot_chase_shoot_ai.h"

namespace bot {

void ChaseShootAI::apply(Robot& robot, GameScreen& screen)
{
    GameMap& map = screen.getMap();
    Player* player = map.getPlayer();

    bool dontApply = robot.testFlag(GAME_OBJ_FLAG_DEAD) ||
                     map.isOutsideViewport(&robot) || 
                     player == nullptr ||
                     player->testFlag(GAME_OBJ_FLAG_DEAD);
                        
    if (dontApply)
    {
        return;
    }

    if (map.isOutsideViewport(&robot))
    {
        resetAction(robot, ACTION_NONE);
        return;
    }

    TimePoint t = Clock::now();
    if (timeDistMs(t, robot.getLastChangeActionTime()) >= m_changeActionIntervalMs)
    {
        tryChangeAction(robot, screen);
    }

    switch (robot.getCurAction())
    {
        case ACTION_CHASE:
            applyChaseAction(robot, screen);
            break;
        case ACTION_SHOOT:
            applyShootAction(robot, screen);
            break;
    }
}

void ChaseShootAI::resetAction(Robot& robot, Action action)
{
    switch (action)
    {
        case ACTION_NONE:
            robot.setCurAction(action);
            robot.setMovingEnabled(false);
            robot.setShootingEnabled(false);
            return;
        case ACTION_SHOOT:
            robot.setCurAction(action);
            robot.setShootingEnabled(true);
            robot.setMovingEnabled(false);
            return;
        case ACTION_CHASE:
            robot.setCurAction(action);
            robot.setMovingEnabled(true);
            robot.setShootingEnabled(false);
            return;
        default:
            LOG_ERROR("Invalid action %d", static_cast<int>(action));
            return;
    }
}

void ChaseShootAI::tryChangeAction(Robot& robot, GameScreen& screen)
{
    const Player* player = screen.getMap().getPlayer();
    float d = dist(robot.getPosX(), robot.getPosY(), player->getPosX(), player->getPosY());

    if (d >= m_distToChase && robot.getMoveAbility())
    {
        resetAction(robot, ACTION_CHASE);
    }
    else
    {
        resetAction(robot, ACTION_SHOOT);
    }
}

void ChaseShootAI::applyChaseAction(Robot& robot, GameScreen& screen)
{
    GameMap& map = screen.getMap();
    const Player* player = map.getPlayer();

    float d = dist(robot.getPosX(), robot.getPosY(), player->getPosX(), player->getPosY());
    if (d <= m_distToChase)
    {
        resetAction(robot, ACTION_SHOOT);
        applyShootAction(robot, screen);
        return;
    }

    float directionX, directionY;
    float speedX, speedY;
    const MoveAbility* moveAbility = robot.getMoveAbility();

    calculateDirection(directionX, directionY, robot.getPosX(), robot.getPosY(), player->getPosX(), player->getPosY());
    speedX = moveAbility->getSpeed() * directionX;
    speedY = moveAbility->getSpeed() * directionY;

    
}

} // end of namespace bot
