#ifndef INCLUDE_BOT_AI_ROBOT_TEMPLATE
#define INCLUDE_BOT_AI_ROBOT_TEMPLATE

#include "gametemplate/bot_robot_template.h"

namespace bot {

class AI;

class AIRobotTemplate : public RobotTemplate {
public:
    AIRobotTemplate() 
        : m_ai(nullptr)
    {}

    AI* getAI() const
    {
        return m_ai;
    }

    void setAI(AI* ai)
    {
        m_ai = ai;
    }

protected:
    AI* m_ai;
};

} // end of namespace bot

#endif
