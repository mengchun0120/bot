#include "misc/bot_log.h"
#include "opengl/bot_text_system.h"
#include "opengl/bot_color.h"
#include "gametemplate/bot_dashboard_template.h"
#include "gameobj/bot_player.h"
#include "gameobj/bot_dashboard.h"

namespace bot {

Dashboard::Dashboard()
    : m_template(nullptr)
    , m_player(nullptr)
{
}

void Dashboard::init(const DashboardTemplate* t, const TextSystem* textSys,
                     float screenHeight, const Player* player)
{
    m_template = t;
    m_textSys = textSys;
    m_player = player;

    initEffectPos(t, screenHeight);
    initHeader(t, textSys, screenHeight);
}

void Dashboard::initEffectPos(const DashboardTemplate* t, float screenHeight)
{
    float y = screenHeight - t->getHeaderTopMargin() - t->getEffectRingRadius();
    float x = t->getEffectStartX() + t->getEffectRingRadius();
    float deltaX = t->getEffectSpacingX() + t->getEffectRingRadius() * 2.0f;

    m_effectPositions.resize(GOODIE_EFFECT_COUNT);
    for (int i = 0; i < GOODIE_EFFECT_COUNT; ++i, x += deltaX)
    {
        m_effectPositions[i].m_pos[0] = x;
        m_effectPositions[i].m_pos[1] = y;
    }
}

void Dashboard::initHeader(const DashboardTemplate* t, const TextSystem* textSys, float screenHeight)
{
    const Rectangle& rect = textSys->getRect(TEXT_SIZE_MEDIUM, ' ');
    float y = screenHeight - t->getHeaderTopMargin();
    m_hpIconPos[0] = t->getHPIconX() + t->getHPRect()->width() / 2.0f;
    m_hpIconPos[1] = y - t->getHPRect()->height() / 2.0f;
    m_hpTextPos[0] = t->getHPTextX();
    m_hpTextPos[1] = y - rect.height();
    m_goldIconPos[0] = t->getGoldIconX() + t->getGoldRect()->width() / 2.0f;
    m_goldIconPos[1] = y - t->getGoldRect()->height() / 2.0f;
    m_goldTextPos[0] = t->getGoldTextX();
    m_goldTextPos[1] = y - rect.height();
}

void Dashboard::draw(SimpleShaderProgram& program)
{
    const float HP_GOOD_THRESHOLD = 2.0f / 3.0f;
    const float HP_BAD_THRESHOLD = 1.0f / 3.0f;
    int i = 0;
    const GoodieEffect* effect;

    for (effect = m_player->getFirstActiveEffect(); effect; effect = effect->getNext(), ++i)
    {
        effect->draw(program, m_effectPositions[i].m_pos);
    }

    m_template->getHPRect()->draw(program, m_hpIconPos, nullptr, nullptr, nullptr,
                                  m_template->getHPTexture()->textureId(), nullptr);

    const Color* hpColor = nullptr;
    float hpRatio = m_player->getHPRatio();
    if (hpRatio >= HP_GOOD_THRESHOLD)
    {
        hpColor = m_template->getHPGoodColor();
    }
    else if (hpRatio >= HP_BAD_THRESHOLD)
    {
        hpColor = m_template->getHPBadColor();
    }
    else
    {
        hpColor = m_template->getHPCriticalColor();
    }

    m_textSys->drawString(program, m_player->getHPStr(), TEXT_SIZE_MEDIUM, m_hpTextPos, 
                          hpColor->getColor());

    m_template->getGoldRect()->draw(program, m_goldIconPos, nullptr, nullptr, nullptr,
                                    m_template->getGoldTexture()->textureId(), nullptr);

    m_textSys->drawString(program, m_player->getGoldStr(), TEXT_SIZE_MEDIUM, m_goldTextPos,
                          m_template->getGoldTextColor()->getColor());
}

} // end of namespace bot
