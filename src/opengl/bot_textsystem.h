#ifndef INCLUDE_BOT_TEXTSYSTEM
#define INCLUDE_BOT_TEXTSYSTEM

#include <unordered_map>
#include <string>
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"

namespace bot {

class SimpleShaderProgram;

class TextSystem {
public:
    enum Size {
        BIG = 0,
        MEDIUM,
        SMALL,
        SIZE_COUNT
    };

    static const int MIN_CHAR = 32;
    static const int MAX_CHAR = 126;
    static const int CHAR_COUNT = MAX_CHAR - MIN_CHAR + 1;

    TextSystem();

    virtual ~TextSystem();

    bool init(const std::string& fontFolder);

    const Texture& getTexture(char ch) const
    {
        return m_textures[static_cast<int>(ch) - MIN_CHAR];
    }

    const Rectangle& getRect(Size sz, char ch) const
    {
        return *(m_rectMap[sz][static_cast<int>(ch) - MIN_CHAR]);
    }

    void drawString(SimpleShaderProgram& program, const std::string& str,
                    Size size, const float *pos, const float *color) const;

    void getStringSize(float &width, float &height, Size sz, const std::string& str) const;

protected:
    std::unordered_map<int, Rectangle> m_rects[SIZE_COUNT];
    Rectangle *m_rectMap[SIZE_COUNT][CHAR_COUNT];
    Texture m_textures[CHAR_COUNT];
};

} // close of namespace bot

#endif
