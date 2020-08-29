#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_map_generator.h"
#include "app/bot_app.h"

namespace bot {

bool GameLib::load()
{
    const AppConfig& cfg = App::getInstance().getConfig();

    if (!m_textureLib.load(cfg.getTextureLib().c_str()))
    {
        LOG_ERROR("Failed to read texture lib from %s", cfg.getTextureLib().c_str());
        return false;
    }
    
    LOG_INFO("Done loading texture library from %s", cfg.getTextureLib().c_str());

    if (!m_rectLib.load(cfg.getRectLib().c_str()))
    {
        LOG_ERROR("Failed to read rect lib from %s", cfg.getRectLib().c_str());
        return false;
    }

    LOG_INFO("Done loading rectangle library from %s", cfg.getRectLib().c_str());
    
    if (!m_colorLib.load(cfg.getColorLib().c_str()))
    {
        LOG_ERROR("Failed to read color lib from %s", cfg.getColorLib().c_str());
        return false;
    }

    LOG_INFO("Done loading color library from %s", cfg.getColorLib().c_str());

    if (!m_tileTemplateLib.load(cfg.getTileTemplateLib().c_str()))
    {
        LOG_ERROR("Failed to read tile template lib from %s", cfg.getTileTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading tile template library from %s", cfg.getTileTemplateLib().c_str());

    if (!m_particleEffectTemplateLib.load(cfg.getParticleEffectTemplateLib().c_str()))
    {
        LOG_ERROR("Failed to read particle effect lib from %s", cfg.getParticleEffectTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading particle effect lib from %s", cfg.getParticleEffectTemplateLib().c_str());

    if (!m_missileTemplateLib.load(cfg.getMissileTemplateLib().c_str()))
    {
        LOG_ERROR("Failed to read missile template lib from %s", cfg.getMissileTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading missile template library from %s", cfg.getMissileTemplateLib().c_str());

    if (!m_progressRingLib.load(cfg.getProgressRingLib().c_str()))
    {
        LOG_ERROR("Failed to read progress ring from %s", cfg.getProgressRingLib().c_str());
        return false;
    }

    LOG_INFO("Done loading progress ring library from %s", cfg.getProgressRingLib().c_str());

    if (!parseVector(m_goodieTemplateLib, cfg.getGoodieTemplateLib().c_str()))
    {
        LOG_ERROR("Failed to read goodie template lib from %s", cfg.getGoodieTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading goodie template lib from %s", cfg.getGoodieTemplateLib().c_str());

    if (!m_aiLib.load(cfg.getAILib().c_str()))
    {
        LOG_ERROR("Failed to read AI from %s", cfg.getAILib().c_str());
        return false;
    }

    LOG_INFO("Done loading ai library from %s", cfg.getAILib().c_str());

    if (!m_aiRobotTemplateLib.load(cfg.getAIRobotTemplateLib().c_str()))
    {
        LOG_ERROR("Failed to read ai-robot template lib from %s", cfg.getAIRobotTemplateLib().c_str());
        return false;
    }

    LOG_INFO("Done loading ai-robot template library from %s", cfg.getAIRobotTemplateLib().c_str());

    if (!m_playerTemplate.init())
    {
        return false;
    }

    MapGenerator::Parser mapGeneratorParser(&m_playerTemplate, m_tileTemplateLib, m_aiRobotTemplateLib);
    if (!m_mapGeneratorLib.load(cfg.getMapGeneratorLib().c_str(), mapGeneratorParser))
    {
        LOG_ERROR("Failed to load map-generator lib from %s", cfg.getMapGeneratorLib().c_str());
        return false;
    }

    LOG_INFO("Done loading map-generator lib from %s", cfg.getMapGeneratorLib().c_str());

    if (!m_dashboardConfig.init())
    {
        return false;
    }
    
    if (!m_buttonConfig.init())
    {
        return false;
    }

    if (!m_messageBoxConfig.init())
    {
        return false;
    }

    if (!m_startScreenConfig.init())
    {
        return false;
    }


    return true;
}

} // end of namespace bot
