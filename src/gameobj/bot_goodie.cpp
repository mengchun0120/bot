#include "misc/bot_log.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_simple_shader_program.h"
#include "screen/bot_game_screen.h"
#include "gameobj/bot_progress_ring.h"
#include "gameobj/bot_goodie.h"

namespace bot {

Goodie::Goodie(const GoodieTemplate* t, float x, float y)
    : GameObject(t)
    , m_startTime(Clock::now())
{
    m_pos[0] = x;
    m_pos[1] = y;
}

Goodie::~Goodie()
{
}

void Goodie::present(ShaderProgram& program)
{
    const GoodieTemplate* t = getTemplate();
    SimpleShaderProgram& p = static_cast<SimpleShaderProgram&>(program);

    t->getRect()->draw(p, m_pos, nullptr, nullptr, nullptr, t->getTexture()->textureId(), nullptr);
}

} // end of namespace bot
