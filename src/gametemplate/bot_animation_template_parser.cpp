#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "misc/bot_file_utils.h"
#include "gametemplate/bot_animation_template.h"
#include "gametemplate/bot_animation_template_parser.h"

namespace bot {

bool AnimationTemplateParser::parse(AnimationTemplate* animationTemplate, const rapidjson::Value& elem)
{
	float coverBreathX = 0.0f, coverBreathY = 0.0f;
	float collideBreathX = 0.0f, collideBreathY = 0.0f;
	bool collidable = false;
	std::string rectName;
	float interval = 0.0f;
	std::vector<std::string> textureFiles;

	std::vector<JsonParseParam> params =
	{
		{&coverBreathX,   "coverBreathX",   JSONTYPE_FLOAT},
		{&coverBreathY,   "coverBreathY",   JSONTYPE_FLOAT},
		{&collideBreathY, "collideBreathY", JSONTYPE_FLOAT},
		{&collideBreathY, "collideBreathY", JSONTYPE_FLOAT},
		{&collidable,     "collidable",     JSONTYPE_BOOL},
		{&rectName,       "rect",           JSONTYPE_STRING},
		{&interval,       "interval",       JSONTYPE_FLOAT},
		{&textureFiles,   "files",          JSONTYPE_STRING_ARRAY}
	};

	if (!parseJson(params, elem)) 
	{
		return false;
	}

	animationTemplate->setCoverBreathX(coverBreathX);
	animationTemplate->setCoverBreathY(coverBreathY);
	animationTemplate->setCollideBreathX(collideBreathX);
	animationTemplate->setCollideBreathY(collideBreathY);
	animationTemplate->setFrameInterval(interval);
	animationTemplate->setFlag(GAME_OBJ_FLAG_INDESTRUCTABLE);

	if (collidable) 
	{
		animationTemplate->setFlag(GAME_OBJ_FLAG_COLLIDABLE);
	}
	else
	{
		animationTemplate->clearFlag(GAME_OBJ_FLAG_COLLIDABLE);
	}

	const Rectangle* rect = m_rectLib.getObjectByName(rectName.c_str());

	if (!rect)
	{
		LOG_ERROR("Cannot find rect %s", rectName.c_str());
		return false;
	}
	animationTemplate->setRect(rect);

	int numFrames = textureFiles.size();

	animationTemplate->setNumFrames(numFrames);
	for (int i = 0; i < numFrames; ++i)
	{
		Texture& frame = animationTemplate->getFrame(i);
		std::string filePath = constructPath({m_animationFolder, textureFiles[i]});

		if (!frame.load(filePath))
		{
			LOG_ERROR("Failed to load frame %s", filePath.c_str());
			return false;
		}
	}

	return true;
}

} // end of namespace bot
