#include "misc/bot_log.h"
#include "gametemplate/bot_texture_parser.h"
#include "gametemplate/bot_rectangle_parser.h"
#include "gametemplate/bot_color_parser.h"
#include "gametemplate/bot_tile_template_parser.h"
#include "gametemplate/bot_missile_template_parser.h"
#include "gametemplate/bot_robot_template_parser.h"
#include "gametemplate/bot_animation_template_parser.h"
#include "gametemplate/bot_player_template_parser.h"
#include "gametemplate/bot_game_template_lib.h"

namespace bot {

GameTemplateLib::GameTemplateLib()
{
}

GameTemplateLib::~GameTemplateLib()
{
}

bool GameTemplateLib::load(const std::string& textureFolder, const std::string& textureLibFile,
	                       const std::string& rectLibFile, const std::string& colorLibFile,
	                       const std::string& tileTemplateLibFile, const std::string& animationFolder,
	                       const std::string& animationTemplateLibFile, const std::string& missileTemplateLibFile,
	                       const std::string& robotTemplateLibFile, const std::string& playerTemplateFile)
{
	TextureParser textureParser(textureFolder);
	if (!readNamedLibFromJson(m_textureLib, textureLibFile.c_str(), textureParser))
	{
		LOG_ERROR("Failed to read texture lib from %s", textureLibFile.c_str());
		return false;
	}
	
	LOG_INFO("Done loading texture library from %s", textureLibFile.c_str());

	RectangleParser rectParser;
	if (!readNamedLibFromJson(m_rectLib, rectLibFile.c_str(), rectParser))
	{
		LOG_ERROR("Failed to read rect lib from %s", rectLibFile.c_str());
		return false;
	}

	LOG_INFO("Done loading rectangle library from %s", rectLibFile.c_str());
	
	ColorParser colorParser;
	if (!readNamedLibFromJson(m_colorLib, colorLibFile.c_str(), colorParser))
	{
		LOG_ERROR("Failed to read color lib from %s", colorLibFile.c_str());
		return false;
	}

	LOG_INFO("Done loading color library from %s", colorLibFile.c_str());

	TileTemplateParser tileParser(m_textureLib, m_rectLib, m_colorLib);
	if (!readNamedLibFromJson(m_tileTemplateLib, tileTemplateLibFile.c_str(), tileParser))
	{
		LOG_ERROR("Failed to read tile template lib from %s", tileTemplateLibFile.c_str());
		return false;
	}

	LOG_INFO("Done loading tile template library from %s", tileTemplateLibFile.c_str());

	/*
	AnimationTemplateParser animationParser(animationFolder, m_rectLib);
	if (!readNamedLibFromJson(m_animationTemplateLib, animationTemplateLibFile.c_str(), animationParser))
	{
		LOG_ERROR("Failed to read animation template lib from %s", animationTemplateLibFile.c_str());
		return false;
	}*/

	MissileTemplateParser missileParser(m_textureLib, m_rectLib, m_colorLib, m_animationTemplateLib);
	if (!readNamedLibFromJson(m_missileTemplateLib, missileTemplateLibFile.c_str(), missileParser))
	{
		LOG_ERROR("Failed to read missile template lib from %s", missileTemplateLibFile.c_str());
		return false;
	}

	LOG_INFO("Done loading missile template library from %s", missileTemplateLibFile.c_str());

	RobotTemplateParser robotParser(m_textureLib, m_rectLib, m_colorLib, m_missileTemplateLib);
	if (!readNamedLibFromJson(m_robotTemplateLib, robotTemplateLibFile.c_str(), robotParser))
	{
		LOG_ERROR("Failed to read robot template lib from %s", robotTemplateLibFile.c_str());
		return false;
	}

	LOG_INFO("Done loading robot template library from %s", robotTemplateLibFile.c_str());

	if (!loadPlayerTemplate(playerTemplateFile))
	{
		LOG_ERROR("Failed to read player template from %s", playerTemplateFile.c_str());
		return false;
	}


	
	return true;
}

bool GameTemplateLib::loadPlayerTemplate(const std::string& playerTemplateFile)
{
	LOG_INFO("Loading player template from %s", playerTemplateFile.c_str());

	rapidjson::Document doc;
	if (!readJson(doc, playerTemplateFile.c_str()))
	{
		return false;
	}

	if (!doc.IsObject())
	{
		LOG_ERROR("Invalid format in %s", playerTemplateFile.c_str());
		return false;
	}

	const rapidjson::Value& playerJson = doc.GetObject();
	PlayerTemplateParser parser(m_textureLib, m_rectLib, m_colorLib, m_missileTemplateLib);

	if (!parser.parse(&m_playerTemplate, playerJson))
	{
		return false;
	}

	LOG_INFO("Done loading player template from %s", playerTemplateFile.c_str());

	return true;
}

} // end of namespace bot
