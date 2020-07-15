#ifndef INCLUDE_BOT_BUTTON_CONFIG
#define INCLUDE_BOT_BUTTON_CONFIG

#include <string>
#include "structure/bot_named_map.h"
#include "opengl/bot_color.h"
#include "opengl/bot_texture.h"

namespace bot {

class Color;
class Texture;

class ButtonConfig {
public:
    ButtonConfig()
        : m_texture(nullptr)
        , m_normalTextColor(nullptr)
        , m_hoverTextColor(nullptr)
        , m_pressTextColor(nullptr)
    {}

    ~ButtonConfig()
    {}

    bool load(const char* fileName, const NamedMap<Texture>& textureLib, const NamedMap<Color>& colorLib);

    const Texture* getTexture() const
    {
        return m_texture;
    }

    const Color* getNormalTextColor() const
    {
        return m_normalTextColor;
    }

    const Color* getHoverTextColor() const
    {
        return m_hoverTextColor;
    }

    const Color* getPressTextColor() const
    {
        return m_pressTextColor;
    }

private:
    const Texture* m_texture;
    const Color* m_normalTextColor;
    const Color* m_hoverTextColor;
    const Color* m_pressTextColor;
};

} // end of namespace bot

#endif
