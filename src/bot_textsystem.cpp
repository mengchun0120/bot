#include <sstream>
#include "bot_log.h"
#include "bot_utils.h"
#include "bot_app.h"

namespace bot {

std::string getTextSystemImageFile(int ch)
{
    std::string file = "ascii_" + std::to_string(ch) + ".png";
    return constructPath({ App::g_app.getResourceDir(), "fonts", file });
}

bool loadTextSystemTextures(Texture *textures)
{
    for(int ch = TextSystem::MIN_CHAR; ch <= TextSystem::MAX_CHAR; ++ch) {
        std::string fileName = getTextSystemImageFile(ch);
        if(!textures[ch-TextSystem::MIN_CHAR].load(fileName)) {
            LOG_ERROR("Failed to create texture for %c", static_cast<char>(ch));
            return false;
        }
    }

    return true;
}

bool loadTextSystemRectangles(std::unordered_map<int, Rectangle> *rects,
                        Rectangle *rectMap[][TextSystem::CHAR_COUNT],
                        const Texture* textures)
{
    float scaleFactor[] = {1.0f, 0.75f, 0.5f};
    int height, width;

    for(int size = static_cast<int>(TextSystem::BIG);
        size < static_cast<int>(TextSystem::SIZE_COUNT); ++size) {

        for(int ch = 0; ch < TextSystem::CHAR_COUNT; ++ch) {
            height = static_cast<int>(textures[ch].height() * scaleFactor[size]);
            width = static_cast<int>(textures[ch].width() * scaleFactor[size]);

            auto it = rects[size].find(width);
            if(it == rects[size].end()) {
                bool ret = rects[size][width].init(static_cast<float>(width),
                                                   static_cast<float>(height),
                                                   true);
                if(!ret) {
                    LOG_ERROR("Failed to create Rectangle for size=%d width=%d",
                              size, width);
                    return false;
                }

                rectMap[size][ch] = &(rects[size][width]);
            } else {
                rectMap[size][ch] = &(it->second);
            }
        }
    }

    return true;
}

TextSystem::TextSystem()
{

}

TextSystem::~TextSystem()
{
}

bool TextSystem::init()
{
    if(!loadTextSystemTextures(m_textures)) {
        return false;
    }

    if(!loadTextSystemRectangles(m_rects, m_rectMap, m_textures)) {
        return false;
    }

    return true;
}

void TextSystem::drawString(SimpleShaderProgram& program, const std::string& str,
                      Size size, const float *pos, const float *color)
{
    if(str.empty()) {
        return;
    }

    program.setUseColor(false);
    program.setUseObjRef(true);

    if(color) {
        program.setUseTexColor(true);
        program.setTexColor(color);
    } else {
        program.setUseTexColor(false);
    }

    float realPos[] = {pos[0], pos[1]};
    Rectangle *rect = m_rectMap[size][str[0]-MIN_CHAR];
    float halfWidth = rect->width() / 2.0f;
    realPos[0] += halfWidth;
    realPos[1] += rect->height() / 2.0f;

    for(int i = 0; i < static_cast<int>(str.size()); ++i) {
        program.setObjRef(realPos);
        program.setPosition(rect->vertexArray());
        program.setTexture(m_textures[str[i]-MIN_CHAR].textureId());
        glDrawArrays(GL_TRIANGLE_FAN, 0, rect->vertexArray().numVertices());

        if(i < static_cast<int>(str.size()) - 1) {
            realPos[0] += halfWidth;
            rect = m_rectMap[size][str[i+1]-MIN_CHAR];
            halfWidth = rect->width() / 2.0f;
            realPos[0] += halfWidth;
            rect = m_rectMap[size][str[i+1]-MIN_CHAR];
        }
    }
}

void TextSystem::getStringSize(float& width, float& height, Size sz, const std::string& str)
{
    Rectangle& rect = getRect(sz, str[0]);
    float w = rect.width();
    height = rect.height();
    for(int i = 1; i < static_cast<int>(str.size()); ++i) {
        Rectangle& r = getRect(sz, str[i]);
        w += r.width();
    }
    width = w;
}

} // close of namespace bot
