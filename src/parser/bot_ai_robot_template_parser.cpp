#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "parser/bot_ai_robot_template_parser.h"

namespace bot {

AIRobotTemplate* AIRobotTemplateParser::parse(const rapidjson::Value& elem)
{
    AIRobotTemplate* aiRobotTemplate = new AIRobotTemplate();

    if (!RobotTemplateParser::parse(aiRobotTemplate, elem))
    {
        delete aiRobotTemplate;
        return nullptr;
    }

    std::string aiName;

    if (!parseJson(aiName, elem, "ai"))
    {
        delete aiRobotTemplate;
        return nullptr;
    }

    AI* ai = m_aiLib.search(aiName);
    if (!ai)
    {
        LOG_ERROR("Cannot find ai %s", aiName.c_str());
        delete aiRobotTemplate;
        return nullptr;
    }

    aiRobotTemplate->setAI(ai);

    return aiRobotTemplate;
}

} // end of namespace bot
