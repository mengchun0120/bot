#include "misc/bot_log.h"
#include "misc/bot_jsonutils.h"
#include "gametemplate/bot_robottemplate.h"
#include "gametemplate/bot_robottemplateparser.h"

namespace bot {

bool RobotTemplateParser::parse(RobotTemplate* robotTemplate, const rapidjson::Value& elem)
{
	if (!parseBaseAttributes(robotTemplate, elem))
	{
		return false;
	}

	if (!parseComponents(robotTemplate, elem))
	{
		return false;
	}

	if (!parseMoveAbility(robotTemplate, elem))
	{
		return false;
	}

	if (!parseShootAbility(robotTemplate, elem))
	{
		return false;
	}

	return true;
}

bool RobotTemplateParser::parseBaseAttributes(RobotTemplate* robotTemplate, const rapidjson::Value& elem)
{
	float coverBreathX = 0.0f, coverBreathY = 0.0f;
	float collideBreathX = 0.0f, collideBreathY = 0.0f;
	int hp = 0;

	std::vector<JsonParseParam> robotParams =
	{
		{&coverBreathX,       "coverBreathX",       JSONTYPE_FLOAT},
		{&coverBreathY,       "coverBreathY",       JSONTYPE_FLOAT},
		{&collideBreathX,     "collideBreathX",     JSONTYPE_FLOAT},
		{&collideBreathY,     "collideBreathY",     JSONTYPE_FLOAT},
		{&hp,                 "hp",                 JSONTYPE_INT}
	};

	if (!parseJson(robotParams, elem))
	{
		return false;
	}

	robotTemplate->setCoverBreathX(coverBreathX);
	robotTemplate->setCoverBreathY(coverBreathY);
	robotTemplate->setCollideBreathX(collideBreathX);
	robotTemplate->setCollideBreathY(collideBreathY);
	robotTemplate->setHP(hp);

	return true;
}

bool RobotTemplateParser::parseComponents(RobotTemplate* robotTemplate, const rapidjson::Value& elem)
{
	if (!elem.HasMember("components"))
	{
		LOG_ERROR("Components missing");
		return false;
	}

	if (!elem["components"].IsArray())
	{
		LOG_ERROR("Components must be an array");
		return false;
	}

	const rapidjson::Value& components = elem["components"];
	int numComponents = components.Capacity();
	std::string textureName, rectName, colorName;
	float componentX = 0.0f, componentY = 0.0f;

	std::vector<JsonParseParam> componentParams =
	{
		{&textureName, "texture",    JSONTYPE_STRING},
		{&rectName,    "rect",       JSONTYPE_STRING},
		{&colorName,   "color",      JSONTYPE_STRING},
		{&componentX,  "componentX", JSONTYPE_FLOAT},
		{&componentY,  "componentY", JSONTYPE_FLOAT}
	};

	robotTemplate->setNumComponents(numComponents);
	for (int i = 0; i < numComponents; ++i)
	{
		if (!parseJson(componentParams, components[i]))
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
			LOG_ERROR("Failed to find rect %s", rectName.c_str());
			return false;
		}

		const Color* color = m_colorLib.getObjectByName(colorName.c_str());
		if (!color)
		{
			LOG_ERROR("Failed to find color %s", colorName.c_str());
			return false;
		}

		robotTemplate->setComponentTexture(i, texture);
		robotTemplate->setComponentRect(i, rect);
		robotTemplate->setComponentColor(i, color);
		robotTemplate->setComponentPosX(i, componentX);
		robotTemplate->setComponentPosY(i, componentY);
	}

	return true;
}

bool RobotTemplateParser::parseMoveAbility(RobotTemplate* robotTemplate, const rapidjson::Value& elem)
{
	bool movable = false;
	float moveSpeed = 0.0f;
	int moveComponentIdx = 0;

	std::vector<JsonParseParam> params =
	{
		{&movable,            "movable",            JSONTYPE_BOOL},
		{&moveSpeed,          "moveSpeed",          JSONTYPE_FLOAT},
		{&moveComponentIdx,   "moveComponentIdx",   JSONTYPE_INT}
	};

	if (!parseJson(params, elem))
	{
		return false;
	}

	if (!movable)
	{
		return true;
	}

	if (moveComponentIdx < 0 || moveComponentIdx >= robotTemplate->getNumComponents())
	{
		LOG_ERROR("moveComponentIdx out of range");
		return false;
	}

	robotTemplate->setMoveAbilityTemplate(moveSpeed);
	robotTemplate->setComponentForMoveAbility(moveComponentIdx);

	return true;
}

bool RobotTemplateParser::parseShootAbility(RobotTemplate* robotTemplate, const rapidjson::Value& elem)
{
	bool shootable = false;
	float shootInterval = 0.0f, shootPosX = 0.0f, shootPosY = 0.0f;
	int shootComponentIdx = 0;
	std::string missileName;

	std::vector<JsonParseParam> params =
	{
		{&shootable,          "shootable",          JSONTYPE_BOOL},
		{&shootInterval,      "shootInterval",      JSONTYPE_FLOAT},
		{&shootPosX,          "shootPosX",          JSONTYPE_FLOAT},
		{&shootPosY,          "shootPosY",          JSONTYPE_FLOAT},
		{&shootComponentIdx,  "shootComponentIdx",  JSONTYPE_INT},
		{&missileName,        "missile",            JSONTYPE_STRING}
	};

	if (!parseJson(params, elem))
	{
		return false;
	}

	if (!shootable)
	{
		return true;
	}

	if (shootComponentIdx < 0 || shootComponentIdx >= robotTemplate->getNumComponents())
	{
		LOG_ERROR("shootComponentIdx out of range");
		return false;
	}

	const MissileTemplate* missile = m_missileTemplateLib.getObjectByName(missileName.c_str());
	if (!missile)
	{
		LOG_ERROR("Couldn't find missile %s", missileName.c_str());
		return false;
	}

	robotTemplate->setShootAbilityTemplate(shootInterval, shootPosX, shootPosY, missile);
	robotTemplate->setComponentForShootAbility(shootComponentIdx);

	return true;
}

} // end of namespace bot
