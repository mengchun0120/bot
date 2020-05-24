#ifndef INCLUDE_BOT_AI
#define INCLUDE_BOT_AI

namespace bot {

class Robot;
class GameScreen;

class AI {
public:
    AI()
    {}

    virtual ~AI()
    {}

    virtual void apply(Robot& robot, float delta, GameScreen& screen) = 0;
};

} // end of namespace bot

#endif
