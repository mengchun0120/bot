#ifndef INCLUDE_BOT_GAME_LIB
#define INCLUDE_BOT_GAME_LIB

#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "ai/bot_ai.h"
#include "gametemplate/bot_tile_template.h"
#include "gametemplate/bot_missile_template.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "gametemplate/bot_animation_template.h"
#include "gametemplate/bot_particle_effect_template.h"
#include "gametemplate/bot_player_template.h"

namespace bot {

class GameLib {
public:
    GameLib();

    ~GameLib();

    bool load(const std::string& textureFolder, const std::string& textureLibFile, 
              const std::string& rectLibFile, const std::string& colorLibFile,
              const std::string& tileTemplateLibFile, const std::string& particleEffectTemplateLibFile,
              const std::string& missileTemplateLibFile, const std::string& aiLibFile, 
              const std::string& aiRobotTemplateLibFile, const std::string& playerTemplateFile);

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

private:
    bool loadPlayerTemplate(const std::string& playerTemplateFile);

private:
    NamedMap<Texture> m_textureLib;
    NamedMap<Rectangle> m_rectLib;
    NamedMap<Color> m_colorLib;
    NamedMap<TileTemplate> m_tileTemplateLib;
    NamedMap<ParticleEffectTemplate> m_particleEffectTemplateLib;
    NamedMap<MissileTemplate> m_missileTemplateLib;
    NamedMap<AI> m_aiLib;
    NamedMap<AIRobotTemplate> m_aiRobotTemplateLib;
    PlayerTemplate* m_playerTemplate;
};

} // end of namespace bot

#endif
