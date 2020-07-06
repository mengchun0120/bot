#include "misc/bot_log.h"
#include "gametemplate/bot_dashboard_template.h"
#include "gameobj/bot_player.h"
#include "gameobj/bot_dashboard.h"

namespace bot {

Dashboard::Dashboard()
    : m_template(nullptr)
    , m_player(nullptr)
{
}

void Dashboard::init(const DashboardTemplate* t, float screenWidth, const Player* player)
{
    m_template = t;
    m_player = player;

    float y = t->getMarginBottom() + t->getRingRadius();
    float x = screenWidth / 2.0f - (GOODIE_EFFECT_COUNT - 1.0f) * (t->getRingRadius() + t->getSpacingX() / 2.0f);
    float deltaX = t->getSpacingX() + t->getRingRadius() * 2.0f;

    m_positions.resize(GOODIE_EFFECT_COUNT);
    for (int i = 0; i < GOODIE_EFFECT_COUNT; ++i, x += deltaX)
    {
        m_positions[i].m_pos[0] = x;
        m_positions[i].m_pos[1] = y;
    }
}

void Dashboard::draw(SimpleShaderProgram& program)
{
    int i = 0;
    const GoodieEffect* effect;

    for (effect = m_player->getFirstActiveEffect(); effect; effect = effect->getNext(), ++i)
    {
        effect->draw(program, m_positions[i].m_pos);
    }
}

} // end of namespace bot
