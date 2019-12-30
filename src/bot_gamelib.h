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

    Texture &getTexture(int idx)
    {
        return m_textureLib[idx];
    }

    Rectangle &getRect(int idx)
    {
        return m_rectLib[idx];
    }

    Color &getColor(int idx)
    {
        return m_colorLib[idx];
    }

    ComponentTemplate &getComponentTemplate(int idx)
    {
        return m_componentTemplateLib[idx];
    }

    GameObjectTemplate &getGameObjectTemplate(int idx)
    {
        return m_gameObjectTemplateLib[idx];
    }

private:
    bool initTextureLib();

    bool initRectLib();

    bool initColorLib();

    bool initComponentTemplateLib();

    bool initGameObjectTemplateLib();

    void organizeGameObjectTemplate();

    bool fillComponentBullet();

private:
    std::vector<Texture> m_textureLib;
    std::vector<Rectangle> m_rectLib;
    std::vector<Color> m_colorLib;
    std::vector<ComponentTemplate> m_componentTemplateLib;
    std::vector<GameObjectTemplate> m_gameObjectTemplateLib;
    std::vector<GameObjectTemplate*> m_tileTemplateLib;
    std::vector<GameObjectTemplate*> m_botTemplateLib;
    std::vector<GameObjectTemplate*> m_bulletTemplateLib;
    NameIdMap m_textureMap;
    NameIdMap m_rectMap;
    NameIdMap m_colorMap;
    NameIdMap m_componentTemplateMap;
    NameIdMap m_gameObjectTemplateMap;
};

} // end of namespace bot

#endif
