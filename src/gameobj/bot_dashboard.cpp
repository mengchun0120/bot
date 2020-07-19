#include "misc/bot_log.h"
#include "opengl/bot_text_system.h"
#include "opengl/bot_color.h"
#include "gameobj/bot_dashboard_config.h"
#include "gameobj/bot_dashboard.h"
#include "gameobj/bot_player.h"
#include "app/bot_app.h"

namespace bot {

Dashboard::Dashboard()
    : m_player(nullptr)
{
}

void Dashboard::init(const Player* player)
{
    m_player = player;

    const App& app = App::getInstance();
    const DashboardConfig& cfg = app.getGameLib().getDashboardConfig();
    float screenHeight = app.getViewportHeight();
    const TextSystem& textSys = app.getTextSystem();

    initEffectPos(cfg, screenHeight);
    initHeader(cfg, textSys, screenHeight);
}

void Dashboard::initEffectPos(const DashboardConfig& cfg, float screenHeight)
{
    float y = screenHeight - cfg.getHeaderTopMargin() - cfg.getEffectRingRadius();
    float x = cfg.getEffectStartX() + cfg.getEffectRingRadius();
    float deltaX = cfg.getEffectSpacingX() + cfg.getEffectRingRadius() * 2.0f;

    m_effectPositions.resize(GOODIE_EFFECT_COUNT);
    for (int i = 0; i < GOODIE_EFFECT_COUNT; ++i, x += deltaX)
    {
        m_effectPositions[i].m_pos[0] = x;
        m_effectPositions[i].m_pos[1] = y;
    }
}

void Dashboard::initHeader(const DashboardConfig& cfg, const TextSystem& textSys, float screenHeight)
{
    const Rectangle& rect = textSys.getRect(TEXT_SIZE_MEDIUM, ' ');
    float y = screenHeight - cfg.getHeaderTopMargin();
    m_hpIconPos[0] = cfg.getHPIconX() + cfg.getHPRect()->width() / 2.0f;
    m_hpIconPos[1] = y - cfg.getHPRect()->height() / 2.0f;
    m_hpTextPos[0] = cfg.getHPTextX();
    m_hpTextPos[1] = y - rect.height();
    m_goldIconPos[0] = cfg.getGoldIconX() + cfg.getGoldRect()->width() / 2.0f;
    m_goldIconPos[1] = y - cfg.getGoldRect()->height() / 2.0f;
    m_goldTextPos[0] = cfg.getGoldTextX();
    m_goldTextPos[1] = y - rect.height();
}

void Dashboard::draw()
{
    App& app = App::getInstance();
    const DashboardConfig& cfg = app.getGameLib().getDashboardConfig();
    const TextSystem& textSys = app.getTextSystem();
    const float HP_GOOD_THRESHOLD = 2.0f / 3.0f;
    const float HP_BAD_THRESHOLD = 1.0f / 3.0f;
    int i = 0;
    const GoodieEffect* effect;

    for (effect = m_player->getFirstActiveEffect(); effect; effect = effect->getNext(), ++i)
    {
        effect->draw(m_effectPositions[i].m_pos);
    }

    cfg.getHPRect()->draw(m_hpIconPos, nullptr, nullptr, nullptr,
                          cfg.getHPTexture()->textureId(), nullptr);

    const Color* hpColor = nullptr;
    float hpRatio = m_player->getHPRatio();
    if (hpRatio >= HP_GOOD_THRESHOLD)
    {
        hpColor = cfg.getHPGoodColor();
    }
    else if (hpRatio >= HP_BAD_THRESHOLD)
    {
        hpColor = cfg.getHPBadColor();
    }
    else
    {
        hpColor = cfg.getHPCriticalColor();
    }

    textSys.drawString(m_player->getHPStr(), TEXT_SIZE_MEDIUM, m_hpTextPos, 
                       hpColor->getColor());

    cfg.getGoldRect()->draw(m_goldIconPos, nullptr, nullptr, nullptr,
                            cfg.getGoldTexture()->textureId(), nullptr);

    textSys.drawString(m_player->getGoldStr(), TEXT_SIZE_MEDIUM, m_goldTextPos,
                       cfg.getGoldTextColor()->getColor());
}

} // end of namespace bot
