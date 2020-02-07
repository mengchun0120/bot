#ifndef INCLUDE_BOT_GAMELIB
#define INCLUDE_BOT_GAMELIB

#include <vector>
#include "bot_nameidmap.h"
#include "bot_texture.h"
#include "bot_rectangle.h"
#include "bot_color.h"
#include "bot_componenttemplate.h"
#include "bot_gameobjecttemplate.h"

namespace bot {

class GameLib {
public:
    GameLib();

    virtual ~GameLib();

    bool init();

    const Texture* getTexture(int idx) const
    {
        return &m_textureLib[idx];
    }

    const Texture* getTexture(const char* name) const;

    const Rectangle* getRect(int idx) const
    {
        return &m_rectLib[idx];
    }

    const Rectangle* getRect(const char* name) const;

    const Color* getColor(int idx) const
    {
        return &m_colorLib[idx];
    }

    const Color* getColor(const char* name) const;

    const ComponentTemplate* getComponentTemplate(int idx) const
    {
        return &m_componentTemplateLib[idx];
    }

    const ComponentTemplate* getComponentTemplate(const char* name) const;

    const GameObjectTemplate* getGameObjectTemplate(int idx) const
    {
        return &m_gameObjectTemplateLib[idx];
    }

    const GameObjectTemplate* getGameObjectTemplate(const char* name) const;

private:
    bool initTextureLib();

    bool initRectLib();

    bool initColorLib();

    bool initComponentTemplateLib();

    bool initGameObjectTemplateLib();

    bool fillComponentBullet();

private:
    std::vector<Texture> m_textureLib;
    std::vector<Rectangle> m_rectLib;
    std::vector<Color> m_colorLib;
    std::vector<ComponentTemplate> m_componentTemplateLib;
    std::vector<GameObjectTemplate> m_gameObjectTemplateLib;
    NameIdMap m_textureMap;
    NameIdMap m_rectMap;
    NameIdMap m_colorMap;
    NameIdMap m_componentTemplateMap;
    NameIdMap m_gameObjectTemplateMap;
};

} // end of namespace bot

#endif
