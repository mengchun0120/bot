#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"

namespace bot {

class Player;
class SimpleShaderProgram;
class DashboardTemplate;

class Dashboard {
public:
    struct Point {
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    };

    Dashboard();

    ~Dashboard()
    {}

    void init(const DashboardTemplate* t, float screenWidth, const Player* player);

    void draw(SimpleShaderProgram& program);

private:
    const DashboardTemplate* m_template;
    const Player* m_player;
    std::vector<Point> m_positions;
};

} // end of namespace bot

#endif
