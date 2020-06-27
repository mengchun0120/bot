#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "parser/bot_texture_parser.h"
#include "parser/bot_rectangle_parser.h"
#include "parser/bot_color_parser.h"
#include "parser/bot_tile_template_parser.h"
#include "parser/bot_particle_effect_template_parser.h"
#include "parser/bot_missile_template_parser.h"
#include "parser/bot_goodie_template_parser.h"
#include "parser/bot_ai_parser.h"
#include "parser/bot_ai_robot_template_parser.h"
#include "parser/bot_player_template_parser.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

GameLib::GameLib()
    : m_playerTemplate(nullptr)
{
}

GameLib::~GameLib()
{
    if (m_playerTemplate)
    {
        delete m_playerTemplate;
    }
}

bool GameLib::load(const std::string& textureFolder, const std::string& textureLibFile,
                   const std::string& rectLibFile, const std::string& colorLibFile,
                   const std::string& tileTemplateLibFile, const std::string& particleEffectTemplateLibFile,
                   const std::string& missileTemplateLibFile, const std::string& goodieTemplateLibFile, 
                   const std::string& aiLibFile, const std::string& aiRobotTemplateLibFile, 
                   const std::string& playerTemplateFile)
{
    TextureParser textureParser(textureFolder);
    if (!parseNamedMap(m_textureLib, textureLibFile.c_str(), textureParser))
    {
        LOG_ERROR("Failed to read texture lib from %s", textureLibFile.c_str());
        return false;
    }
    
    LOG_INFO("Done loading texture library from %s", textureLibFile.c_str());

    RectangleParser rectParser;
    if (!parseNamedMap(m_rectLib, rectLibFile.c_str(), rectParser))
    {
        LOG_ERROR("Failed to read rect lib from %s", rectLibFile.c_str());
        return false;
    }

    LOG_INFO("Done loading rectangle library from %s", rectLibFile.c_str());
    
    ColorParser colorParser;
    if (!parseNamedMap(m_colorLib, colorLibFile.c_str(), colorParser))
    {
        LOG_ERROR("Failed to read color lib from %s", colorLibFile.c_str());
        return false;
    }

    LOG_INFO("Done loading color library from %s", colorLibFile.c_str());

    TileTemplateParser tileParser(m_textureLib, m_rectLib, m_colorLib);
    if (!parseNamedMap(m_tileTemplateLib, tileTemplateLibFile.c_str(), tileParser))
    {
        LOG_ERROR("Failed to read tile template lib from %s", tileTemplateLibFile.c_str());
        return false;
    }

    LOG_INFO("Done loading tile template library from %s", tileTemplateLibFile.c_str());

    ParticleEffectTemplateParser particleEffectParser(m_textureLib, m_colorLib);
    if (!parseNamedMap(m_particleEffectTemplateLib, particleEffectTemplateLibFile.c_str(), particleEffectParser))
    {
        LOG_ERROR("Failed to read particle effect lib from %s", particleEffectTemplateLibFile.c_str());
        return false;
    }

    LOG_INFO("Done loading particle effect lib from %s", particleEffectTemplateLibFile.c_str());

    MissileTemplateParser missileParser(m_textureLib, m_rectLib, m_colorLib, m_particleEffectTemplateLib);
    if (!parseNamedMap(m_missileTemplateLib, missileTemplateLibFile.c_str(), missileParser))
    {
        LOG_ERROR("Failed to read missile template lib from %s", missileTemplateLibFile.c_str());
        return false;
    }

    LOG_INFO("Done loading missile template library from %s", missileTemplateLibFile.c_str());

    GoodieTemplateParser goodieParser(m_textureLib, m_rectLib);
    if (!parseVector(m_goodieTemplateLib, goodieTemplateLibFile.c_str(), goodieParser))
    {
        LOG_ERROR("Failed to read goodie template lib from %s", goodieTemplateLibFile.c_str());
        return false;
    }

    LOG_INFO("Done loading goodie template lib from %s", goodieTemplateLibFile.c_str());

    AIParser aiParser;
    if (!parseNamedMap(m_aiLib, aiLibFile.c_str(), aiParser))
    {
        LOG_ERROR("Failed to read AI from %s", aiLibFile.c_str());
        return false;
    }

    LOG_INFO("Done loading ai library from %s", aiLibFile.c_str());

    AIRobotTemplateParser aiRobotParser(m_textureLib, m_rectLib, m_colorLib, m_missileTemplateLib, m_aiLib);
    if (!parseNamedMap(m_aiRobotTemplateLib, aiRobotTemplateLibFile.c_str(), aiRobotParser))
    {
        LOG_ERROR("Failed to read ai-robot template lib from %s", aiRobotTemplateLibFile.c_str());
        return false;
    }

    LOG_INFO("Done loading ai-robot template library from %s", aiRobotTemplateLibFile.c_str());

    if (!loadPlayerTemplate(playerTemplateFile))
    {
        LOG_ERROR("Failed to read player template from %s", playerTemplateFile.c_str());
        return false;
    }
    
    LOG_INFO("Done loading player template from %s", playerTemplateFile.c_str());

    return true;
}

bool GameLib::loadPlayerTemplate(const std::string& playerTemplateFile)
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

    m_playerTemplate = parser.parse(playerJson);
    if (!m_playerTemplate)
    {
        return false;
    }

    LOG_INFO("Done loading player template from %s", playerTemplateFile.c_str());

    return true;
}

} // end of namespace bot
