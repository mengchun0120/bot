#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_tile_template.h"
#include "app/bot_app.h"

namespace bot {

TileTemplate* TileTemplate::create(const rapidjson::Value& elem)
{
    TileTemplate* t = new TileTemplate();
    if (!t->init(elem))
    {
        delete t;
        return nullptr;
    }
    return t;
}

TileTemplate::TileTemplate()
	: GameObjectTemplate(GAME_OBJ_TYPE_TILE)
	, m_texture(nullptr)
	, m_rect(nullptr)
	, m_color(nullptr)
	, m_hp(0)
{
}

bool TileTemplate::init(const rapidjson::Value& elem)
{
    std::string textureName, rectName, colorName;
    bool indestructable = false;

    std::vector<JsonParseParam> params =
    {
        {&textureName,        "texture",        JSONTYPE_STRING},
        {&rectName,           "rect",           JSONTYPE_STRING},
        {&colorName,          "color",          JSONTYPE_STRING},
        {&m_coverBreathX,     "coverBreathX",   JSONTYPE_FLOAT},
        {&m_coverBreathY,     "coverBreathY",   JSONTYPE_FLOAT},
        {&m_collideBreathX,   "collideBreathX", JSONTYPE_FLOAT},
        {&m_collideBreathY,   "collideBreathY", JSONTYPE_FLOAT},
        {&m_hp,               "hp",             JSONTYPE_INT},
        {&indestructable,     "indestructable", JSONTYPE_BOOL}
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
        LOG_ERROR("Failed to find rectangle %s", rectName.c_str());
        return false;
    }

    m_color = lib.getColor(colorName);
    if (!m_color)
    {
        LOG_ERROR("Failed to find color %s", colorName.c_str());
        return false;
    }

    if (indestructable)
    {
        setFlag(GAME_OBJ_FLAG_INDESTRUCTABLE);
    }
    else
    {
        clearFlag(GAME_OBJ_FLAG_INDESTRUCTABLE);
    }

    return true;
}

} // end of namespace bot
