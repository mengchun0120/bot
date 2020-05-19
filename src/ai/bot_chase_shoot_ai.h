#ifndef INCLUDE_BOT_CHASE_SHOOT_AI
#define INCLUDE_BOT_CHASE_SHOOT_AI

#include "ai/bot_ai.h"

namespace bot {

class ChaseShootAI : public AI {
public:
    ChaseShootAI(float distToChase, float changeActionIntervalMs)
        : AI(changeActionIntervalMs)
        , m_distToChase(distToChase)
    {}

    virtual ~ChaseShootAI()
    {}

    virtual void apply(Robot& robot, GameScreen& screen);

protected:
    void resetAction(Robot& robot, Action action);

    void tryChangeAction(Robot& robot, GameScreen& screen);

    void applyChaseAction(Robot& robot, GameScreen& screen);

    void applyShootAction(Robot& robot, GameScreen& screen);

protected:
    
};

} // end of namespace bot

#endif
