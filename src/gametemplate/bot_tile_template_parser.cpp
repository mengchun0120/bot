#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_tile_template.h"
#include "gametemplate/bot_tile_template_parser.h"

namespace bot {

bool TileTemplateParser::parse(TileTemplate* tileTemplate, const rapidjson::Value& elem)
{
	std::string textureName, rectName, colorName;
	float coverBreathX = 0.0f, coverBreathY = 0.0f;
	float collideBreathX = 0.0f, collideBreathY = 0.0f;
	int hp = 0;
	bool indestructable = false;

	std::vector<JsonParseParam> params = 
	{
		{&textureName,    "texture",        JSONTYPE_STRING},
		{&rectName,       "rect",           JSONTYPE_STRING},
		{&colorName,      "color",          JSONTYPE_STRING},
		{&coverBreathX,   "coverBreathX",   JSONTYPE_FLOAT},
		{&coverBreathY,   "coverBreathY",   JSONTYPE_FLOAT},
		{&collideBreathX, "collideBreathX", JSONTYPE_FLOAT},
		{&collideBreathY, "collideBreathY", JSONTYPE_FLOAT},
		{&hp,             "hp",             JSONTYPE_INT},
		{&indestructable, "indestructable", JSONTYPE_BOOL}
	};
	
	if (!parseJson(params, elem)) 
	{
		return false;
	}

	const Texture* texture = m_textureLib.getObjectByName(textureName.c_str());
	if (!texture) 
	{
		LOG_ERROR("Failed to find texture %s", textureName.c_str());
		return false;
	}

	const Rectangle* rect = m_rectLib.getObjectByName(rectName.c_str());
	if (!rect) 
	{
		LOG_ERROR("Failed to find rectangle %s", rectName.c_str());
		return false;
	}

	const Color* color = m_colorLib.getObjectByName(colorName.c_str());
	if (!color)
	{
		LOG_ERROR("Failed to find color %s", colorName.c_str());
		return false;
	}

	tileTemplate->setCoverBreathX(coverBreathX);
	tileTemplate->setCoverBreathY(coverBreathY);
	tileTemplate->setCollideBreathX(collideBreathX);
	tileTemplate->setCollideBreathY(collideBreathY);
	tileTemplate->setHP(hp);
	
	if (indestructable) 
	{
		tileTemplate->setFlag(GAME_OBJ_FLAG_INDESTRUCTABLE);
	}
	else 
	{
		tileTemplate->clearFlag(GAME_OBJ_FLAG_INDESTRUCTABLE);
	}
	
	tileTemplate->setTexture(texture);
	tileTemplate->setRect(rect);
	tileTemplate->setColor(color);

	return true;
}

} // end of namespace bot
