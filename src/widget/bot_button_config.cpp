#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "widget/bot_button_config.h"

namespace bot {

bool ButtonConfig::load(const char* fileName, const NamedMap<Texture>& textureLib, const NamedMap<Color>& colorLib)
{
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

    const rapidjson::Value& cfg = doc.GetObject();
    std::string textureName, normalColorName, hoverColorName, pressColorName;

    std::vector<JsonParseParam> params = {
        {&textureName,     "texture",         JSONTYPE_STRING},
        {&normalColorName, "normalTextColor", JSONTYPE_STRING},
        {&hoverColorName,  "hoverTextColor",  JSONTYPE_STRING},
        {&pressColorName,  "pressTextColor",   JSONTYPE_STRING}
    };

    if (!parseJson(params, cfg))
    {
        return false;
    }

    m_texture = textureLib.search(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Failed to find texture %s", textureName.c_str());
        return false;
    }

    m_normalTextColor = colorLib.search(normalColorName);
    if (!m_normalTextColor)
    {
        LOG_ERROR("Failed to find color %s", normalColorName.c_str());
        return false;
    }

    m_hoverTextColor = colorLib.search(hoverColorName);
    if (!m_hoverTextColor)
    {
        LOG_ERROR("Failed to find color %s", hoverColorName.c_str());
        return false;
    }

    m_pressTextColor = colorLib.search(pressColorName);
    if (!m_pressTextColor)
    {
        LOG_ERROR("Failed to find color %s", pressColorName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
