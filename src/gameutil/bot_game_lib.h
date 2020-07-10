#ifndef INCLUDE_BOT_GAME_LIB
#define INCLUDE_BOT_GAME_LIB

#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "ai/bot_ai.h"
#include "gametemplate/bot_tile_template.h"
#include "gametemplate/bot_missile_template.h"
#include "gametemplate/bot_goodie_template.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "gametemplate/bot_animation_template.h"
#include "gametemplate/bot_particle_effect_template.h"
#include "gameobj/bot_progress_ring.h"
#include "gametemplate/bot_player_template.h"
#include "gametemplate/bot_dashboard_template.h"

namespace bot {

class AppConfig;

class GameLib {
public:
    GameLib();

    ~GameLib();

    bool load(const AppConfig& cfg);

    const Texture* getTexture(const std::string& name) const
    {
        return m_textureLib.search(name);
    }

    const Rectangle* getRect(const std::string& name) const
    {
        return m_rectLib.search(name);
    }

    const Color* getColor(const std::string& name) const
    {
        return m_colorLib.search(name);
    }

    const TileTemplate* getTileTemplate(const std::string& name) const
    {
        return m_tileTemplateLib.search(name);
    }

    const ParticleEffectTemplate* getParticleEffectTemplate(const std::string& name) const
    {
        return m_particleEffectTemplateLib.search(name);
    }

    const MissileTemplate* getMissileTemplate(const std::string& name) const
    {
        return m_missileTemplateLib.search(name);
    }

    const AI* getAI(const std::string& name) const
    {
        return m_aiLib.search(name);
    }

    const AIRobotTemplate* getAIRobotTemplate(const std::string& name) const
    {
        return m_aiRobotTemplateLib.search(name);
    }

    const PlayerTemplate* getPlayerTemplate() const
    {
        return m_playerTemplate;
    }

    const std::vector<GoodieTemplate>& getGoodieTemplateLib() const
    {
        return m_goodieTemplateLib;
    }

    const GoodieTemplate* getGoodieTemplate(int goodieIdx) const
    {
        return &m_goodieTemplateLib[goodieIdx];
    }

    const ProgressRing* getProgressRing(const std::string& name) const
    {
        return m_progressRingLib.search(name);
    }

    const DashboardTemplate& getDashboardTemplate() const
    {
        return m_dashboardTemplate;
    }

private:
    bool loadPlayerTemplate(const std::string& playerTemplateFile);

private:
    static GameLib g_gameLib;

    NamedMap<Texture> m_textureLib;
    NamedMap<Rectangle> m_rectLib;
    NamedMap<Color> m_colorLib;
    NamedMap<TileTemplate> m_tileTemplateLib;
    NamedMap<ParticleEffectTemplate> m_particleEffectTemplateLib;
    NamedMap<MissileTemplate> m_missileTemplateLib;
    std::vector<GoodieTemplate> m_goodieTemplateLib;
    NamedMap<AI> m_aiLib;
    NamedMap<AIRobotTemplate> m_aiRobotTemplateLib;
    NamedMap<ProgressRing> m_progressRingLib;
    PlayerTemplate* m_playerTemplate;
    DashboardTemplate m_dashboardTemplate;
};

} // end of namespace bot

#endif
