#include "misc/bot_mathutils.h"
#include "misc/bot_log.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "gameutil/bot_collide.h"
#include "screen/bot_gamescreen.h"
#include "gameobj/bot_robot.h"

namespace bot {

Robot::Robot(const RobotTemplate* t)
    : GameObject(t)
    , m_hp(t->getHP())
    , m_side(-1)
{
    m_direction[0] = 1.0f;
    m_direction[1] = 0.0f;

    initComponents();
    initAbilities();
}

Robot::~Robot()
{
    for (int i = 0; i < NUM_ABILITY_TYPES; ++i)
    {
        if (!m_abilities[i])
        {
            delete m_abilities[i];
        }
    }
}

void Robot::initComponents()
{
    const RobotTemplate* t = getTemplate();
    int numComponents = t->getNumComponents();
    
    m_components.resize(numComponents);
    for (int i = 0; i < numComponents; ++i)
    {
        Component& c = m_components[i];
        c.m_pos[0] = m_pos[0] + t->getComponentPosX(i);
        c.m_pos[1] = m_pos[1] + t->getComponentPosY(i);
    }
}

void Robot::initAbilities()
{
    for (int i = 0; i < NUM_ABILITY_TYPES; ++i)
    {
        m_abilities[i] = nullptr;
    }

    const RobotTemplate* t = getTemplate();

    const MoveAbilityTemplate* moveTemplate = t->getMoveAbilityTemplate();
    if (moveTemplate)
    {
        MoveAbility* moveAbility = new MoveAbility(moveTemplate);
        m_abilities[ABILITY_MOVE] = moveAbility;
    }

    const ShootAbilityTemplate* shootTemplate = t->getShootAbilityTemplate();
    if (shootTemplate)
    {
        ShootAbility* shootAbility = new ShootAbility(shootTemplate);
        m_abilities[ABILITY_SHOOT] = shootAbility;
        resetShootPos();
    }
}

void Robot::present(SimpleShaderProgram& program)
{
    const RobotTemplate* t = getTemplate();
    int count = static_cast<int>(m_components.size());

    for (int i = 0; i < count; ++i) {
        const RobotTemplate::ComponentTemplate& ct = t->getComponent(i);
        ct.m_rect->draw(program, m_components[i].m_pos, m_direction, nullptr, nullptr, 
                        ct.m_texture->textureId(), ct.m_color);
    }
}

bool Robot::update(float delta, GameScreen& screen)
{
    if (!updateMoveAbility(delta, screen))
    {
        return false;
    }

    return true;
}

void Robot::shiftPos(float deltaX, float deltaY)
{
    m_pos[0] += deltaX;
    m_pos[1] += deltaY;

    int numComponents = static_cast<int>(m_components.size());
    for (int i = 0; i < numComponents; ++i)
    {
        m_components[i].m_pos[0] += deltaX;
        m_components[i].m_pos[1] += deltaY;
    }

    ShootAbility* shootAbility = getShootAbility();
    if (shootAbility)
    {
        shootAbility->shiftShootPos(deltaX, deltaY);
    }
}

void Robot::setPos(float x, float y)
{
    shiftPos(x - m_pos[0], y - m_pos[1]);
}

void Robot::setDirection(float directionX, float directionY)
{
    m_direction[0] = directionX;
    m_direction[1] = directionY;

    const RobotTemplate* t = getTemplate();
    int numComponents = static_cast<int>(m_components.size());

    for (int i = 0; i < numComponents; ++i)
    {
        const RobotTemplate::ComponentTemplate& ct = t->getComponent(i);
        float x = ct.m_pos[0], y = ct.m_pos[1];

        rotate(x, y, directionX, directionY);
        m_components[i].m_pos[0] = m_pos[0] + x;
        m_components[i].m_pos[1] = m_pos[1] + y;
    }

    ShootAbility* shootAbility = getShootAbility();
    if (shootAbility)
    {
        Component* c = getComponentForShootAbility();
        shootAbility->setShootPosDirection(c->m_pos[0], c->m_pos[1], directionX, directionY);
    }
}

bool Robot::addHP(int deltaHP)
{
    if (testFlag(GAME_OBJ_FLAG_INDESTRUCTABLE))
    {
        return true;
    }

    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return false;
    }

    m_hp = clamp(m_hp + deltaHP, 0, getTemplate()->getHP());

    return m_hp > 0;
}

Robot::Component* Robot::getComponentForMoveAbility()
{
    const RobotTemplate::ComponentTemplate* t = getTemplate()->getComponentForMoveAbility();
    if (!t)
    {
        return nullptr;
    }
    return &m_components[t->m_index];
}

bool Robot::resetShootPos()
{
    ShootAbility* shootAbility = getShootAbility();
    if (!shootAbility)
    {
        return false;
    }
    
    Component* c = getComponentForShootAbility();
    shootAbility->setShootPosDirection(c->m_pos[0], c->m_pos[1], m_direction[0], m_direction[1]);

    return true;
}

Robot::Component* Robot::getComponentForShootAbility()
{
    const RobotTemplate::ComponentTemplate* t = getTemplate()->getComponentForShootAbility();
    if (!t)
    {
        return nullptr;
    }
    return &m_components[t->m_index];
}

bool Robot::setDestAndDirection(float destX, float destY)
{
    float directionX, directionY;

    calculateDirection(directionX, directionY, m_pos[0], m_pos[1], destX, destY);
    setDirection(directionX, directionY);

    MoveAbility* moveAbility = getMoveAbility();
    if (!moveAbility) 
    {
        return false;
    }

    moveAbility->setDest(destX, destY);

    return true;
}

bool Robot::setMovingEnabled(bool enabled)
{
    MoveAbility* moveAbility = getMoveAbility();
    if (!moveAbility) 
    {
        return false;
    }

    moveAbility->setMoving(enabled);

    return true;
}

bool Robot::isMoving() const
{
    const MoveAbility* moveAbility = getMoveAbility();
    if (!moveAbility) 
    {
        return false;
    }

    return moveAbility->isMoving();
}

bool Robot::updateMoveAbility(float delta, GameScreen& gameScreen)
{
    MoveAbility* moveAbility = getMoveAbility();
    if (!moveAbility || !moveAbility->isMoving())
    {
        return true;
    }

    float speedX = moveAbility->getSpeed() * m_direction[0];
    float speedY = moveAbility->getSpeed() * m_direction[1];
    float newDelta;
    GameMap::MapCell collideObjs;
    GameMap& map = gameScreen.getMap();

    bool touch1 = checkTouchBoundary(newDelta, map.getMapWidth(), map.getMapHeight(), m_pos[0], m_pos[1],
                                    getCollideBreathX(), getCollideBreathY(), speedX, speedY, delta);

    bool touch2 = map.checkCollisionWithObjects(newDelta, collideObjs, this, speedX, speedY, delta);
    
    if (!collideObjs.isEmpty())
    {
        // process objects collided with this object
        map.freeMapCell(collideObjs);
    }

    shiftPos(speedX * newDelta, speedY * newDelta);
    /*if (moveAbility->hasDest())
    {
        moveAbility->checkDest(m_pos[0], m_pos[1]);
    }*/

    if (touch1 || touch2) {
        moveAbility->setMoving(false);
        //moveAbility->setHasDest(false);
    }
    
    map.repositionObject(this);

    return true;
}

/*

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
*/
} // end of namespace bot