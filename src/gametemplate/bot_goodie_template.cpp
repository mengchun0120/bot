#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_goodie_template.h"
#include "app/bot_app.h"

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

GoodieTemplate* GoodieTemplate::create(const rapidjson::Value& elem)
{
    GoodieTemplate* t = new GoodieTemplate();
    if (!t->init(elem))
    {
        delete t;
        return nullptr;
    }
    return t;
}

bool GoodieTemplate::init(const rapidjson::Value& elem)
{
    std::string textureName, rectName, effectRectName, typeName, ringName;

    std::vector<JsonParseParam> params =
    {
        {&typeName,         "type",           JSONTYPE_STRING},
        {&textureName,      "texture",        JSONTYPE_STRING},
        {&rectName,         "rect",           JSONTYPE_STRING},
        {&effectRectName,   "effectRect",     JSONTYPE_STRING},
        {&ringName,         "progressRing",   JSONTYPE_STRING},
        {&m_coverBreathX,   "coverBreathX",   JSONTYPE_FLOAT},
        {&m_coverBreathY,   "coverBreathY",   JSONTYPE_FLOAT},
        {&m_collideBreathX, "collideBreathX", JSONTYPE_FLOAT},
        {&m_collideBreathY, "collideBreathY", JSONTYPE_FLOAT},
        {&m_duration,       "duration",       JSONTYPE_FLOAT},
        {&m_weight,         "weight",         JSONTYPE_FLOAT}
    };

    if (!parseJson(params, elem))
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

    m_rect = lib.getRect(rectName);
    if (!m_rect)
    {
        LOG_ERROR("Failed to find rect %s", rectName.c_str());
        return false;
    }

    m_effectRect = nullptr;
    if (!effectRectName.empty())
    {
        m_effectRect = lib.getRect(effectRectName);
        if (!m_effectRect)
        {
            LOG_ERROR("Failed to find effect rect %s", effectRectName.c_str());
            return false;
        }
    }

    m_goodieType = parseGoodieType(typeName);
    if (m_goodieType == GOODIE_UNKNOWN)
    {
        LOG_ERROR("Invalid goodie type: %s", typeName.c_str());
        return false;
    }

    m_ring = nullptr;
    if (!ringName.empty())
    {
        m_ring = lib.getProgressRing(ringName);
        if (!m_ring)
        {
            LOG_ERROR("Failed to find progress ring %s", ringName.c_str());
            return false;
        }
    }

    return true;
}

} // end of namespace bot
