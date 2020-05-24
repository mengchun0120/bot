#include "gameobj/bot_ai_robot.h"

namespace bot {

AIRobot::AIRobot(const AIRobotTemplate* t)
    : Robot(t)
{
}

void AIRobot::present(ShaderProgram& program)
{
    Robot::present(program);
}

void AIRobot::update(float delta, GameScreen& screen)
{
    const AIRobotTemplate* t = getTemplate();
    t->getAI()->apply(*this, delta, screen);
}

} // end of namespace bot
