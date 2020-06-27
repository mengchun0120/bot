#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_goodie_template.h"
#include "parser/bot_goodie_template_parser.h"

namespace bot {

GoodieType parseGoodieType(const std::string& typeName)
{
    if (typeName == "gold")
    {
        return GOODIE_GOLD;
    }
    else if (typeName == "health")
    {
        return GOODIE_HEALTH;
    }
    else if (typeName == "indestructable")
    {
        return GOODIE_INDESTRUCTABLE;
    }
    else if (typeName == "double_experience")
    {
        return GOODIE_DOUBLE_EXPERIENCE;
    }
    else if (typeName == "quick_shooter")
    {
        return GOODIE_QUICK_SHOOTER;
    }
    else if (typeName == "quick_mover")
    {
        return GOODIE_QUICK_MOVER;
    }
    else if (typeName == "double_damage")
    {
        return GOODIE_DOUBLE_DAMAGE;
    }

    return GOODIE_UNKNOWN;       
}

bool GoodieTemplateParser::parse(GoodieTemplate& t, const rapidjson::Value& elem)
{
    std::string textureName, rectName, typeName;
    float coverBreathX = 0.0f, coverBreathY = 0.0f;
    float collideBreathX = 0.0f, collideBreathY = 0.0f;
    float liveDuration = 0.0f, effectDuration = 0.0f;
    float weight = 0.0f;

    std::vector<JsonParseParam> params =
    {
        {&typeName,       "type",           JSONTYPE_STRING},
        {&textureName,    "texture",        JSONTYPE_STRING},
        {&rectName,       "rect",           JSONTYPE_STRING},
        {&coverBreathX,   "coverBreathX",   JSONTYPE_FLOAT},
        {&coverBreathY,   "coverBreathY",   JSONTYPE_FLOAT},
        {&collideBreathX, "collideBreathX", JSONTYPE_FLOAT},
        {&collideBreathY, "collideBreathY", JSONTYPE_FLOAT},
        {&liveDuration,   "liveDuration",   JSONTYPE_FLOAT},
        {&effectDuration, "effectDuration", JSONTYPE_FLOAT},
        {&weight,         "weight",         JSONTYPE_FLOAT}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const Texture* texture = m_textureLib.search(textureName);
    if (!texture)
    {
        LOG_ERROR("Failed to find texture %s", textureName.c_str());
        return false;
    }

    const Rectangle* rect = m_rectLib.search(rectName);
    if (!rect)
    {
        LOG_ERROR("Failed to find rect %s", rectName.c_str());
        return false;
    }

    GoodieType type = parseGoodieType(typeName);
    if (type == GOODIE_UNKNOWN)
    {
        LOG_ERROR("Invalid goodie type: %s", typeName.c_str());
        return false;
    }

    t.setCoverBreathX(coverBreathX);
    t.setCoverBreathY(coverBreathY);
    t.setCollideBreathX(collideBreathX);
    t.setCollideBreathY(collideBreathY);
    t.setGoodieType(type);
    t.setRect(rect);
    t.setTexture(texture);
    t.setLiveDuration(liveDuration);
    t.setEffectDuration(effectDuration);
    t.setWeight(weight);

    return true;
}

} // end of namespace bot