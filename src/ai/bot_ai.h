#ifndef INCLUDE_BOT_AI
#define INCLUDE_BOT_AI

namespace bot {

class Robot;
class GameScreen;

class AI {
public:
    AI(float changeActionIntervalMs)
        : m_changeActionIntervalMs(changeActionIntervalMs)
    {}

    virtual ~AI()
    {}

    virtual void apply(Robot& robot, GameScreen& screen) = 0;

protected:
    float m_changeActionIntervalMs;
};

} // end of namespace bot

#endif
