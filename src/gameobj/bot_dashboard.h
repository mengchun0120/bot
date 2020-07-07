#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"

namespace bot {

class Player;
class SimpleShaderProgram;
class DashboardTemplate;
class TextSystem;

class Dashboard {
public:
    struct Point {
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    };

    Dashboard();

    ~Dashboard()
    {}

    void init(const DashboardTemplate* t, const TextSystem* textSys, float screenHeight, const Player* player);

    void draw(SimpleShaderProgram& program);

private:
    void initEffectPos(const DashboardTemplate* t, float screenHeight);

    void initHeader(const DashboardTemplate* t, const TextSystem* textSys, float screenHeight);

private:
    const DashboardTemplate* m_template;
    const TextSystem* m_textSys;
    const Player* m_player;
    std::vector<Point> m_effectPositions;
    float m_hpIconPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_hpTextPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_goldIconPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_goldTextPos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif
