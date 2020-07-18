#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"

namespace bot {

class Player;
class DashboardConfig;
class TextSystem;

class Dashboard {
public:
    struct Point {
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    };

    Dashboard();

    ~Dashboard()
    {}

    void init(const Player* player);

    void draw();

private:
    void initEffectPos(const DashboardConfig& cfg, float screenHeight);

    void initHeader(const DashboardConfig& cfg, const TextSystem& textSys, float screenHeight);

private:
    const Player* m_player;
    std::vector<Point> m_effectPositions;
    float m_hpIconPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_hpTextPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_goldIconPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_goldTextPos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif
