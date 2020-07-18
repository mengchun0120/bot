#include "misc/bot_json_utils.h"
#include "app/bot_app.h"

namespace bot {

bool DashboardConfig::init()
{
    const AppConfig& cfg = App::getInstance().getConfig();
    const char* fileName = cfg.getDashboardConfigFile().c_str();
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

    std::string hpRectName, hpTextureName;
    std::string hpGoodColorName, hpBadColorName, hpCriticalColorName;
    std::string goldRectName, goldTextureName, goldTextColorName;

    std::vector<JsonParseParam> params = {
        {&hpRectName,            "hpRect",             JSONTYPE_STRING},
        {&hpTextureName,         "hpTexture",          JSONTYPE_STRING},
        {&hpGoodColorName,       "hpGoodColor",        JSONTYPE_STRING},
        {&hpBadColorName,        "hpBadColor",         JSONTYPE_STRING},
        {&hpCriticalColorName,   "hpCriticalColor",    JSONTYPE_STRING},
        {&m_hpIconX,             "hpIconX",            JSONTYPE_FLOAT},
        {&m_hpTextX,             "hpTextX",            JSONTYPE_FLOAT},
        {&goldRectName,          "goldRect",           JSONTYPE_STRING},
        {&goldTextureName,       "goldTexture",        JSONTYPE_STRING},
        {&goldTextColorName,     "goldTextColor",      JSONTYPE_STRING},
        {&m_goldIconX,           "goldIconX",          JSONTYPE_FLOAT},
        {&m_goldTextX,           "goldTextX",          JSONTYPE_FLOAT},
        {&m_effectSpacingX,      "effectSpacingX",     JSONTYPE_FLOAT},
        {&m_effectStartX,        "effectStartX",       JSONTYPE_FLOAT},
        {&m_effectRingRadius,    "effectRingRadius",   JSONTYPE_FLOAT},
        {&m_headerTopMargin,     "headerTopMargin",    JSONTYPE_FLOAT}
    };

    if (!parseJson(params, val))
    {
        return false;
    }

    const GameLib& lib = App::getInstance().getGameLib();

    m_hpRect = lib.getRect(hpRectName);
    if (!m_hpRect)
    {
        LOG_ERROR("Failed to find hp rect %s", hpRectName.c_str());
        return false;
    }

    m_hpTexture = lib.getTexture(hpTextureName);
    if (!m_hpTexture)
    {
        LOG_ERROR("Failed to find hp texture %s", hpTextureName.c_str());
        return false;
    }

    m_hpGoodColor = lib.getColor(hpGoodColorName);
    if (!m_hpGoodColor)
    {
        LOG_ERROR("Failed to find hp good color %s", hpGoodColorName.c_str());
        return false;
    }

    m_hpBadColor = lib.getColor(hpBadColorName);
    if (!m_hpBadColor)
    {
        LOG_ERROR("Failed to find hp bad color %s", hpBadColorName.c_str());
        return false;
    }

    m_hpCriticalColor = lib.getColor(hpCriticalColorName);
    if (!m_hpCriticalColor)
    {
        LOG_ERROR("Failed to find hp critical color %s", hpCriticalColorName.c_str());
        return false;
    }

    m_goldRect = lib.getRect(goldRectName);
    if (!m_goldRect)
    {
        LOG_ERROR("Failed to find gold rect %s", goldRectName.c_str());
        return false;
    }

    m_goldTexture = lib.getTexture(goldTextureName);
    if (!m_goldTexture)
    {
        LOG_ERROR("Failed to find gold texture %s", goldTextureName.c_str());
        return false;
    }

    m_goldTextColor = lib.getColor(goldTextColorName);
    if (!m_goldTextColor)
    {
        LOG_ERROR("Failed to find gold text color %s", goldTextColorName.c_str());
        return false;
    }

    LOG_INFO("Done loading dashboard config from %s", fileName);

    return true;
}

} // end of namespace bot
