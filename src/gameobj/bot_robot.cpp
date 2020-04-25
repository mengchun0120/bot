#include "misc/bot_mathutils.h"
#include "misc/bot_log.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
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
        LOG_INFO("Draw component %d", i);
        const RobotTemplate::ComponentTemplate& ct = t->getComponent(i);
        ct.m_rect->draw(program, m_components[i].m_pos, m_direction, nullptr, nullptr, 
                        ct.m_texture->textureId(), ct.m_color);
    }
}

bool Robot::update(float delta, GameScreen& screen)
{
    return true;
}

void Robot::setPos(float x, float y)
{
    float deltaX = x - m_pos[0];
    float deltaY = y - m_pos[1];

    m_pos[0] = x;
    m_pos[1] = y;

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

/*
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