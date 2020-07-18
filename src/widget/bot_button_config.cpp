#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "widget/bot_button_config.h"
#include "app/bot_app.h"

namespace bot {

bool ButtonConfig::init()
{
    const AppConfig& cfg = App::getInstance().getConfig();
    const char* fileName = cfg.getButtonConfigFile().c_str();
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

    const rapidjson::Value& val = doc.GetObject();
    std::string textureName, normalColorName, hoverColorName, pressColorName;

    std::vector<JsonParseParam> params = {
        {&textureName,     "texture",         JSONTYPE_STRING},
        {&normalColorName, "normalTextColor", JSONTYPE_STRING},
        {&hoverColorName,  "hoverTextColor",  JSONTYPE_STRING},
        {&pressColorName,  "pressTextColor",   JSONTYPE_STRING}
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    const GameLib& lib = App::getInstance().getGameLib();

    m_texture = lib.getTexture(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Failed to find texture %s", textureName.c_str());
        return false;
    }

    m_normalTextColor = lib.getColor(normalColorName);
    if (!m_normalTextColor)
    {
        LOG_ERROR("Failed to find color %s", normalColorName.c_str());
        return false;
    }

    m_hoverTextColor = lib.getColor(hoverColorName);
    if (!m_hoverTextColor)
    {
        LOG_ERROR("Failed to find color %s", hoverColorName.c_str());
        return false;
    }

    m_pressTextColor = lib.getColor(pressColorName);
    if (!m_pressTextColor)
    {
        LOG_ERROR("Failed to find color %s", pressColorName.c_str());
        return false;
    }

    LOG_INFO("Done loading button config from %s", fileName);

    return true;
}

} // end of namespace bot
