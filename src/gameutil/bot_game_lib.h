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
#include "gametemplate/bot_player_template.h"
#include "gameutil/bot_map_generator.h"
#include "gameobj/bot_progress_ring.h"
#include "gameobj/bot_dashboard_config.h"
#include "widget/bot_button_config.h"
#include "widget/bot_message_box_config.h"
#include "screen/bot_start_screen_config.h"

namespace bot {
   
template <typename T> class NamedMap;
class Texture;
class Color;
class Rectangle;
class TileTemplate;
class MissileTemplate;
class GoodieTemplate;
class AI;
class AIRobotTemplate;
class ParticleEffectTemplate;
class ProgressRing;
class MapGenerator;
class AppConfig;

class GameLib {
public:
    GameLib()
    {}

    ~GameLib()
    {}

    bool load(float viewportWidth, float viewportHeight, const AppConfig& cfg);

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

    AI* getAI(const std::string& name)
    {
        return m_aiLib.search(name);
    }

    const AIRobotTemplate* getAIRobotTemplate(const std::string& name) const
    {
        return m_aiRobotTemplateLib.search(name);
    }

    const PlayerTemplate& getPlayerTemplate() const
    {
        return m_playerTemplate;
    }

    const NamedMap<GoodieTemplate>& getGoodieTemplateLib() const
    {
        return m_goodieTemplateLib;
    }

    const ProgressRing* getProgressRing(const std::string& name) const
    {
        return m_progressRingLib.search(name);
    }

    const DashboardConfig& getDashboardConfig() const
    {
        return m_dashboardConfig;
    }

    const ButtonConfig& getButtonConfig() const
    {
        return m_buttonConfig;
    }

    const MessageBoxConfig& getMessageBoxConfig() const
    {
        return m_messageBoxConfig;
    }

    const StartScreenConfig& getStartScreenConfig() const
    {
        return m_startScreenConfig;
    }

    const MapGenerator* getMapGenerator(const std::string& name) const
    {
        return m_mapGeneratorLib.search(name);
    }

private:
    NamedMap<Texture> m_textureLib;
    NamedMap<Rectangle> m_rectLib;
    NamedMap<Color> m_colorLib;
    NamedMap<TileTemplate> m_tileTemplateLib;
    NamedMap<ParticleEffectTemplate> m_particleEffectTemplateLib;
    NamedMap<MissileTemplate> m_missileTemplateLib;
    NamedMap<GoodieTemplate> m_goodieTemplateLib;
    NamedMap<AI> m_aiLib;
    NamedMap<AIRobotTemplate> m_aiRobotTemplateLib;
    NamedMap<ProgressRing> m_progressRingLib;
    PlayerTemplate m_playerTemplate;
    NamedMap<MapGenerator> m_mapGeneratorLib;
    DashboardConfig m_dashboardConfig;
    ButtonConfig m_buttonConfig;
    MessageBoxConfig m_messageBoxConfig;
    StartScreenConfig m_startScreenConfig;
};

} // end of namespace bot

#endif
