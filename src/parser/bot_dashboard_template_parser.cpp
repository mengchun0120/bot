#include <rapidjson/document.h>
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_dashboard_template.h"
#include "parser/bot_dashboard_template_parser.h"

namespace bot {

bool DashboardTemplateParser::parse(DashboardTemplate& t, const std::string& fileName)
{
    rapidjson::Document doc;

    if (!readJson(doc, fileName.c_str()))
    {
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Invalid file format %s", fileName.c_str());
        return false;
    }

    const rapidjson::Value& cfg = doc.GetObject();
    std::string hpRectName, hpTextureName, hpTextColorName;
    std::string goldRectName, goldTextureName, goldTextColorName;
    float effectSpacingX, effectStartX, effectRingRadius;
    float hpIconX, hpTextX, goldIconX, goldTextX, headerTopMargin;

    std::vector<JsonParseParam> params = {
        {&hpRectName,         "hpRect",             JSONTYPE_STRING},
        {&hpTextureName,      "hpTexture",          JSONTYPE_STRING},
        {&hpTextColorName,    "hpTextColor",        JSONTYPE_STRING},
        {&hpIconX,            "hpIconX",            JSONTYPE_FLOAT},
        {&hpTextX,            "hpTextX",            JSONTYPE_FLOAT},
        {&goldRectName,       "goldRect",           JSONTYPE_STRING},
        {&goldTextureName,    "goldTexture",        JSONTYPE_STRING},
        {&goldTextColorName,  "goldTextColor",      JSONTYPE_STRING},
        {&goldIconX,          "goldIconX",          JSONTYPE_FLOAT},
        {&goldTextX,          "goldTextX",          JSONTYPE_FLOAT},
        {&effectSpacingX,     "effectSpacingX",     JSONTYPE_FLOAT},
        {&effectStartX,       "effectStartX",       JSONTYPE_FLOAT},
        {&effectRingRadius,   "effectRingRadius",   JSONTYPE_FLOAT},
        {&headerTopMargin,    "headerTopMargin",    JSONTYPE_FLOAT}
    };

    if (!parseJson(params, cfg))
    {
        return false;
    }

    const Rectangle* hpRect = m_rectLib.search(hpRectName);
    if (!hpRect)
    {
        LOG_ERROR("Failed to find hp rect %s", hpRectName.c_str());
        return false;
    }

    const Texture* hpTexture = m_textureLib.search(hpTextureName);
    if (!hpTexture)
    {
        LOG_ERROR("Failed to find hp texture %s", hpTextureName.c_str());
        return false;
    }

    const Color* hpTextColor = m_colorLib.search(hpTextColorName);
    if (!hpTextColor)
    {
        LOG_ERROR("Failed to find hp text color %s", hpTextColorName.c_str());
        return false;
    }

    const Rectangle* goldRect = m_rectLib.search(goldRectName);
    if (!goldRect)
    {
        LOG_ERROR("Failed to find gold rect %s", goldRectName.c_str());
        return false;
    }

    const Texture* goldTexture = m_textureLib.search(goldTextureName);
    if (!goldTexture)
    {
        LOG_ERROR("Failed to find gold texture %s", goldTextureName.c_str());
        return false;
    }

    const Color* goldTextColor = m_colorLib.search(goldTextColorName);
    if (!goldTextColor)
    {
        LOG_ERROR("Failed to find gold text color %s", goldTextColorName.c_str());
        return false;
    }

    t.setHPRect(hpRect);
    t.setHPTexture(hpTexture);
    t.setHPIconX(hpIconX);
    t.setHPTextX(hpTextX);
    t.setHPTextColor(hpTextColor);
    t.setGoldRect(goldRect);
    t.setGoldTexture(goldTexture);
    t.setGoldIconX(goldIconX);
    t.setGoldTextX(goldTextX);
    t.setGoldTextColor(goldTextColor);
    t.setEffectSpacingX(effectSpacingX);
    t.setEffectStartX(effectStartX);
    t.setEffectRingRadius(effectRingRadius);
    t.setHeaderTopMargin(headerTopMargin);

    return true;
}

} // end of namespace bot
