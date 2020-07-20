#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "screen/bot_start_screen_config.h"
#include "app/bot_app.h"

namespace bot {

bool StartScreenConfig::init()
{
    const AppConfig& cfg = App::getInstance().getConfig();
    const char* fileName = cfg.getStartScreenConfigFile().c_str();
    rapidjson::Document doc;

    if (!readJson(doc, fileName))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid file format %s", fileName);
        return false;
    }

    const rapidjson::Value& jsonCfg = doc.GetObject();
    std::string buttonRectName;
    std::vector<JsonParseParam> params = {
        {&m_buttonSpacing,  "buttonSpacing", JSONTYPE_FLOAT},
        {&buttonRectName,   "buttonRect",    JSONTYPE_STRING},
        {&m_buttonTexts,    "buttonTexts",   JSONTYPE_STRING_ARRAY}
    };

    if (!parseJson(params, jsonCfg))
    {
        LOG_ERROR("Failed to parse start screen config from %s", fileName);
        return false;
    }

    const GameLib& lib = App::getInstance().getGameLib();
    
    m_rect = lib.getRect(buttonRectName);
    if (!m_rect)
    {
        LOG_ERROR("Failed to find button %s", buttonRectName.c_str());
        return false;
    }

    LOG_INFO("Done loading start screen config from %s", fileName);

    return true;
}

} // end of namespace bot
