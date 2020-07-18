#ifndef INCLUDE_BOT_AI_ROBOT_TEMPLATE
#define INCLUDE_BOT_AI_ROBOT_TEMPLATE

#include <rapidjson/document.h>
#include "gametemplate/bot_robot_template.h"

namespace bot {

class AI;

class AIRobotTemplate : public RobotTemplate {
public:
    static AIRobotTemplate* create(const rapidjson::Value& elem);
    
    AIRobotTemplate() 
        : m_ai(nullptr)
    {}

    bool init(const rapidjson::Value& elem);

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
