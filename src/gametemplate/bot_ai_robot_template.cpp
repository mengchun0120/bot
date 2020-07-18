#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "app/bot_app.h"

namespace bot {

AIRobotTemplate* AIRobotTemplate::create(const rapidjson::Value& elem)
{
    AIRobotTemplate* t = new AIRobotTemplate();
    if (!t->init(elem))
    {
        delete t;
        return nullptr;
    }
    return t;
}

bool AIRobotTemplate::init(const rapidjson::Value& elem)
{
    if (!RobotTemplate::init(elem))
    {
        return false;
    }

    std::string aiName;

    if (!parseJson(aiName, elem, "ai"))
    {
        return false;
    }

    GameLib& lib = App::getInstance().getGameLib();

    m_ai = lib.getAI(aiName);
    if (!m_ai)
    {
        LOG_ERROR("Failed to find AI %s", aiName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
