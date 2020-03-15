#include "gameobj/bot_robot.h"

namespace bot {

GameObject::GameObject(const GameObjectTemplate* t)
    : m_template(t)
    , m_coverStartRow(-1)
    , m_coverEndRow(-1)
    , m_coverStartCol(-1)
    , m_coverEndCol(-1)
    , m_flags(0)
    , m_hp(t->getHP())
    , m_side(-1)
{
    m_base.init(t->getBaseComponent(), 0.0f, 0.0f, 1.0f, 0.0f);

    int numParts = t->numParts();

    m_components.resize(numParts);
    for (int i = 0; i < numParts; ++i) {
        const GameObjectTemplate::Part& p = t->getPart(i);
        m_components[i].init(p.m_component, p.m_pos[0], p.m_pos[1], p.m_direction[0], p.m_direction[1]);
    }
}

void GameObject::present()
{
    m_base.present();

    int count = static_cast<int>(m_components.size());
    for (int i = 0; i < count; ++i) {
        m_components[i].present();
    }
}

void GameObject::setPos(float x, float y)
{
    float deltaX = x - m_base.getX();
    float deltaY = y - m_base.getY();
    move(deltaX, deltaY);
}

void GameObject::move(float deltaX, float deltaY)
{
    m_base.move(deltaX, deltaY);
    int count = static_cast<int>(m_components.size());
    for (int i = 0; i < count; ++i) {
        m_components[i].move(deltaX, deltaY);
    }
}

void GameObject::setDirection(float directionX, float directionY)
{
    m_base.setDirection(directionX, directionY);

    int count = static_cast<int>(m_components.size());
    for (int i = 0; i < count; ++i) {
        const GameObjectTemplate::Part& part = m_template->getPart(i);
        float x = part.m_pos[0];
        float y = part.m_pos[1];
        rotate(x, y, directionX, directionY);
        x += m_base.getX();
        y += m_base.getY();
        m_components[i].setPos(x, y);
        m_components[i].setDirection(directionX, directionY);
    }
}

void GameObject::setDirectionByDest(float destX, float destY)
{
    float directionX, directionY;
    getDirection(directionX, directionY, m_base.getX(), m_base.getY(), destX, destY);
    setDirection(directionX, directionY);
}

bool GameObject::setDest(float destX, float destY)
{
    MoveAbility* moveAbility = static_cast<MoveAbility*>(m_base.getAbility(ABILITY_MOVE));
    if (!moveAbility) {
        return false;
    }

    moveAbility->setDestX(destX);
    moveAbility->setDestY(destY);

    return true;
}

bool GameObject::setMovability(bool moving)
{
    MoveAbility* moveAbility = static_cast<MoveAbility*>(m_base.getAbility(ABILITY_MOVE));
    if (!moveAbility) {
        return false;
    }

    moveAbility->setMoving(moving);

    return true;
}

bool GameObject::isMoving() const
{
    MoveAbility* moveAbility = static_cast<MoveAbility*>(m_base.getAbility(ABILITY_MOVE));
    if (!moveAbility) {
        return false;
    }

    return moveAbility->isMoving();
}

float GameObject::getSpeed() const
{
    MoveAbility* moveAbility = static_cast<MoveAbility*>(m_base.getAbility(ABILITY_MOVE));
    if (!moveAbility) {
        return 0.0f;
    }

    return moveAbility->getSpeed();
}

FireAbility* GameObject::getFireAbility() const
{
    FireAbility* fireAbility = static_cast<FireAbility*>(m_base.getAbility(ABILITY_FIRE));
    if (fireAbility) {
        return fireAbility;
    }

    int numComponents = getNumComponents();
    for (int i = 0; i < numComponents; ++i) {
        FireAbility* fireAbility = static_cast<FireAbility*>(m_components[i].getAbility(ABILITY_FIRE));
        if (fireAbility) {
            return fireAbility;
        }
    }

    return nullptr;
}

bool GameObject::startFiring()
{
    FireAbility* fireAbility = getFireAbility();
    if (fireAbility) {
        fireAbility->startFiring();
        return true;
    }
    return false;
}

bool GameObject::endFiring()
{
    FireAbility* fireAbility = getFireAbility();
    if (fireAbility) {
        fireAbility->stopFiring();
        return true;
    }
    return false;
}

bool GameObject::update(float delta, GameScreen& screen)
{


    return false;
}

void GameObject::updateFireAbility(float delta, const Clock::time_point& t, GameScreen& screen)
{
    updateFireAbilityComponent(m_base, delta, t, screen);

    int numComponents = getNumComponents();
    for (int c = 0; c < numComponents; ++c) {
        updateFireAbilityComponent(m_components[c], delta, t, screen);
    }
}

void GameObject::updateFireAbilityComponent(Component& component, float delta, const Clock::time_point& t,
    GameScreen& screen)
{
    FireAbility* fireAbility = static_cast<FireAbility*>(component.getAbility(ABILITY_FIRE));
    if (!fireAbility || !fireAbility->shouldFire(t)) {
        return;
    }

    bool explodeOnCreation = false;
    float x = fireAbility->getFirePosX();
    float y = fireAbility->getFirePosY();
    const GameObjectTemplate& bulletTemplate = fireAbility->getBulletTemplate();

    if (screen.checkOutsideMap(x, y)) {
        explodeOnCreation = true;
    }
    else {
        float collideLeft = x - bulletTemplate.getCollideBreathX();
        float collideRight = x + bulletTemplate.getCollideBreathX();
        float collideBottom = y - bulletTemplate.getCollideBreathY();
        float collideTop = y + bulletTemplate.getCollideBreathY();

        bool collide = screen.testCollision(collideLeft, collideBottom, collideRight, collideTop,
            m_side, GAMEOBJ_BULLET);
        if (collide) {
            explodeOnCreation = true;
        }
    }

    if (explodeOnCreation) {
        // create explosion
    }
    else {
        GameObject* bullet = new GameObject(&bulletTemplate);
        bullet->setPos(x, y);
        bullet->setDirection(getDirectionX(), getDirectionY());
        bullet->setMovability(true);
        screen.addGameObj(bullet);
    }
}

int GameObject::updateMoveAbility(float delta, GameScreen& screen)
{
    MoveAbility* moveAbility = static_cast<MoveAbility*>(m_base.getAbility(ABILITY_MOVE));
    if (!moveAbility || !moveAbility->isMoving()) {
        return 0;
    }

    float speed = moveAbility->getSpeed();
    float speedX = speed * getDirectionX();
    float speedY = speed * getDirectionY();
    float finalDelta;
    bool stopMoving = false;

    bool touchBoundary = checkTouchBoundary(finalDelta, screen.getMapWidth(), screen.getMapHeight(),
        getPosX(), getPosY(), getCollideBreathX(), getCollideBreathY(),
        speedX, speedY, delta);
    if (touchBoundary) {
        stopMoving = true;
    }


}

} // end of namespace bot