#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "app/bot_app_config.h"
#include "parser/bot_texture_parser.h"
#include "parser/bot_rectangle_parser.h"
#include "parser/bot_color_parser.h"
#include "parser/bot_tile_template_parser.h"
#include "parser/bot_particle_effect_template_parser.h"
#include "parser/bot_missile_template_parser.h"
#include "parser/bot_goodie_template_parser.h"
#include "parser/bot_ai_parser.h"
#include "parser/bot_ai_robot_template_parser.h"
#include "parser/bot_progress_ring_parser.h"
#include "parser/bot_player_template_parser.h"
#include "parser/bot_dashboard_template_parser.h"
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

bool GameLib::load(const AppConfig& cfg)
{
    TextureParser textureParser(cfg.getTextureDir());
    if (!parseNamedMap(m_textureLib, cfg.getTextureLib().c_str(), textureParser))
    {
        LOG_ERROR("Failed to read texture lib from %s", cfg.getTextureLib().c_str());
        return false;
    }
    
    LOG_INFO("Done loading texture library from %s", cfg.getTextureLib().c_str());

    RectangleParser rectParser;
    if (!parseNamedMap(m_rectLib, cfg.getRectLib().c_str(), rectParser))
    {
        LOG_ERROR("Failed to read rect lib from %s", cfg.getRectLib().c_str());
        return false;
    }

    LOG_INFO("Done loading rectangle library from %s", cfg.getRectLib().c_str());
    
    ColorParser colorParser;
    if (!parseNamedMap(m_colorLib, cfg.getColorLib().c_str(), colorParser))
    {
        LOG_ERROR("Failed to read color lib from %s", cfg.getColorLib().c_str());
        return false;
    }

    LOG_INFO("Done loading color library from %s", cfg.getColorLib().c_str());

    TileTemplateParser tileParser(m_textureLib, m_rectLib, m_colorLib);
    if (!parseNamedMap(m_tileTemplateLib, cfg.getTileTemplateLib().c_str(), tileParser))
    {
        LOG_ERROR("Failed to read tile template lib from %s", cfg.getTileTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading tile template library from %s", cfg.getTileTemplateLib().c_str());

    ParticleEffectTemplateParser particleEffectParser(m_textureLib, m_colorLib);
    if (!parseNamedMap(m_particleEffectTemplateLib, cfg.getParticleEffectTemplateLib().c_str(), particleEffectParser))
    {
        LOG_ERROR("Failed to read particle effect lib from %s", cfg.getParticleEffectTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading particle effect lib from %s", cfg.getParticleEffectTemplateLib().c_str());

    MissileTemplateParser missileParser(m_textureLib, m_rectLib, m_colorLib, m_particleEffectTemplateLib);
    if (!parseNamedMap(m_missileTemplateLib, cfg.getMissileTemplateLib().c_str(), missileParser))
    {
        LOG_ERROR("Failed to read missile template lib from %s", cfg.getMissileTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading missile template library from %s", cfg.getMissileTemplateLib().c_str());

    ProgressRingParser progressRingParser(m_colorLib);
    if (!parseNamedMap(m_progressRingLib, cfg.getProgressRingLib().c_str(), progressRingParser))
    {
        LOG_ERROR("Failed to read progress ring from %s", cfg.getProgressRingLib().c_str());
        return false;
    }

    LOG_INFO("Done loading progress ring library from %s", cfg.getProgressRingLib().c_str());

    GoodieTemplateParser goodieParser(m_textureLib, m_rectLib, m_progressRingLib);
    if (!parseVector(m_goodieTemplateLib, cfg.getGoodieTemplateLib().c_str(), goodieParser))
    {
        LOG_ERROR("Failed to read goodie template lib from %s", cfg.getGoodieTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading goodie template lib from %s", cfg.getGoodieTemplateLib().c_str());

    AIParser aiParser;
    if (!parseNamedMap(m_aiLib, cfg.getAILib().c_str(), aiParser))
    {
        LOG_ERROR("Failed to read AI from %s", cfg.getAILib().c_str());
        return false;
    }

    LOG_INFO("Done loading ai library from %s", cfg.getAILib().c_str());

    AIRobotTemplateParser aiRobotParser(m_textureLib, m_rectLib, m_colorLib, m_missileTemplateLib, m_aiLib);
    if (!parseNamedMap(m_aiRobotTemplateLib, cfg.getAIRobotTemplateLib().c_str(), aiRobotParser))
    {
        LOG_ERROR("Failed to read ai-robot template lib from %s", cfg.getAIRobotTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading ai-robot template library from %s", cfg.getAIRobotTemplateLib().c_str());

    if (!loadPlayerTemplate(cfg.getPlayerTemplateLib()))
    {
        LOG_ERROR("Failed to read player template from %s", cfg.getPlayerTemplateLib().c_str());
        return false;
    }
    
    DashboardTemplateParser dashboardParser(m_rectLib, m_textureLib, m_colorLib);
    if (!dashboardParser.parse(m_dashboardTemplate, cfg.getDashboardTemplate()))
    {
        LOG_ERROR("Failed to read dashboard template from %s", cfg.getDashboardTemplate().c_str());
        return false;
    }

    LOG_INFO("Done loading dashboard template from %s", cfg.getDashboardTemplate().c_str());

    if (!m_buttonConfig.load(cfg.getButtonConfigFile().c_str(), m_textureLib, m_colorLib))
    {
        LOG_ERROR("Failed to read button config from %s", cfg.getButtonConfigFile().c_str());
        return false;
    }

    LOG_INFO("Done loading button config from %s", cfg.getButtonConfigFile().c_str());

    if (!m_startScreenConfig.load())
    {
        LOG_ERROR("Failed to read start screen config");
        return false;
    }

    LOG_INFO("Done loading start screen config from %s", cfg.getStartScreenConfigFile().c_str());

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
