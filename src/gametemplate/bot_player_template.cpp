#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_player_template.h"
#include "app/bot_app.h"

namespace bot {

PlayerTemplate::PlayerTemplate()
	: RobotTemplate()
{

}

PlayerTemplate::~PlayerTemplate()
{
}

bool PlayerTemplate::init()
{
    const AppConfig& cfg = App::getInstance().getConfig();
    const char* fileName = cfg.getPlayerTemplateLib().c_str();

    rapidjson::Document doc;
    if (!readJson(doc, fileName))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid format in %s", fileName);
        return false;
    }

    const rapidjson::Value& playerJson = doc.GetObject();

    if (!RobotTemplate::init(playerJson))
    {
        return false;
    }

    return true;
}

} // end of namespace bot
