#include <string>
#include "misc/bot_log.h"
#include "gametemplate/bot_missiletemplate.h"
#include "gametemplate/bot_missiletemplateparser.h"

namespace bot {

bool MissileTemplateParser::parse(MissileTemplate* missileTemplate, const rapidjson::Value& elem)
{
	float coverBreathX = 0.0f;
	float coverBreathY = 0.0f;
	float collideBreathX = 0.0f;
	float collideBreathY = 0.0f;
	float speed = 0.0f;
	int explosionPower;
	float explosionBreath;
	std::string textureName;
	std::string rectName;
	std::string colorName;
	std::string explosionAnimation;

	std::vector<JsonParseParam> params =
	{
		{&coverBreathX,       "coverBreathX",       JSONTYPE_FLOAT},
		{&coverBreathY,       "coverBreathY",       JSONTYPE_FLOAT},
		{&collideBreathX,     "collideBreathX",     JSONTYPE_FLOAT},
		{&collideBreathY,     "collideBreathY",     JSONTYPE_FLOAT},
		{&speed,              "speed",              JSONTYPE_FLOAT},
		{&explosionPower,     "explosionPower",     JSONTYPE_INT},
		{&explosionBreath,    "explosionBreath",    JSONTYPE_FLOAT},
		{&textureName,        "texture",            JSONTYPE_STRING},
		{&rectName,           "rect",               JSONTYPE_STRING},
		{&colorName,          "color",              JSONTYPE_STRING},
		{&explosionAnimation, "explosionAnimation", JSONTYPE_STRING}
	};

	if (!parseJson(params, elem)) 
	{
		return false;
	}

	missileTemplate->setCoverBreathX(coverBreathX);
	missileTemplate->setCoverBreathY(coverBreathY);
	missileTemplate->setCollideBreathX(collideBreathX);
	missileTemplate->setCollideBreathY(collideBreathY);
	missileTemplate->setSpeed(speed);
	missileTemplate->setExplosionPower(explosionPower);
	missileTemplate->setExplosionBreath(explosionBreath);
	
	const Texture* texture = m_textureLib.getObjectByName(textureName.c_str());
	
	if (!texture)
	{
		LOG_ERROR("Couldn't find texture %s", textureName.c_str());
		return false;
	}

	missileTemplate->setTexture(texture);

	const Rectangle* rect = m_rectLib.getObjectByName(rectName.c_str());

	if (!rect)
	{
		LOG_ERROR("Couldn't find rect %s", rectName.c_str());
		return false;
	}

	missileTemplate->setRect(rect);

	const Color* color = m_colorLib.getObjectByName(colorName.c_str());

	if (!color)
	{
		LOG_ERROR("Coundn't find color %s", colorName.c_str());
		return false;
	}

	missileTemplate->setColor(color);

	/*
	const AnimationTemplate* explosionTemplate = m_animationTemplateLib.getObjectByName(explosionAnimation.c_str());

	if (!explosionTemplate) 
	{
		LOG_ERROR("Couldn't find animation %s", explosionAnimation.c_str());
		return false;
	}

	missileTemplate->setExplosionTemplate(explosionTemplate);
	*/

	return true;
}

} // end of namespace bot
