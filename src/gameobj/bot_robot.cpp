#include "misc/bot_math_utils.h"
#include "misc/bot_log.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "gameutil/bot_collide.h"
#include "screen/bot_game_screen.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_robot.h"

namespace bot {

Robot::Robot(const RobotTemplate* t)
    : GameObject(t)
    , m_hp(t->getHP())
    , m_side(SIDE_UNKNOWN)
{
    m_direction[0] = 1.0f;
    m_direction[1] = 0.0f;
    m_flags = t->getFlags();

    initComponents();
    initAbilities();
}

Robot::~Robot()
{
    for (int i = 0; i < static_cast<int>(AbilityType::NUM_OF_ABILITIES); ++i)
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
    for (int i = 0; i < static_cast<int>(AbilityType::NUM_OF_ABILITIES); ++i)
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
    updateMoveAbility(delta, screen);

    if (testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return false;
    }

    updateShootAbility(screen);

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

bool Robot::setShootingEnabled(bool enabled)
{
    ShootAbility* shootAbility = getShootAbility();
    if (!shootAbility)
    {
        return false;
    }

    shootAbility->enableShooting(enabled);

    return true;
}

bool Robot::isShooting() const
{
    const ShootAbility* shootAbility = getShootAbility();
    if (!shootAbility)
    {
        return false;
    }

    return shootAbility->isShootingEnabled();
}

void Robot::updateMoveAbility(float delta, GameScreen& gameScreen)
{
    MoveAbility* moveAbility = getMoveAbility();
    if (!moveAbility || !moveAbility->isMoving())
    {
        return;
    }

    float speedX = moveAbility->getSpeed() * m_direction[0];
    float speedY = moveAbility->getSpeed() * m_direction[1];
    float newDelta;
    LinkedList<GameObjectItem> collideObjs;
    GameMap& map = gameScreen.getMap();

    bool collide = map.checkCollision(newDelta, collideObjs, this, speedX, speedY, delta);

    if (!collideObjs.isEmpty())
    {
        processCollisions(collideObjs, gameScreen);
        map.freeGameObjList(collideObjs);
    }

    shiftPos(speedX * newDelta, speedY * newDelta);

    if (collide) {
        moveAbility->setMoving(false);
    }
    
    map.repositionObject(this);
}

void Robot::updateShootAbility(GameScreen& gameScreen)
{
    ShootAbility* shootAbility = getShootAbility();
    if (!shootAbility || !shootAbility->canShoot())
    {
        return;
    }

    GameObjectManager& gameObjManager = gameScreen.getGameObjManager();
    Missile* missile = gameObjManager.createMissile(shootAbility->getMissileTemplate(), this, 
                                                    shootAbility->getShootPosX(), shootAbility->getShootPosY(),
                                                    m_direction[0], m_direction[1], m_side);
    shootAbility->setShootTime();

    GameMap& map = gameScreen.getMap();
    ReturnCode rc = map.checkCollision(missile);

    if (rc == RET_CODE_OUT_OF_SIGHT)
    {
        LOG_INFO("missile out-of-sight");
        gameObjManager.sendToDeathQueue(missile);
        return;
    }

    if (rc == RET_CODE_COLLIDE)
    {
        LOG_INFO("missile explode");
        missile->explode(gameScreen);
        return;
    }

    map.addObject(missile);
}

void Robot::processCollisions(LinkedList<GameObjectItem>& collideObjs, GameScreen& gameScreen)
{
    for (GameObjectItem* item = collideObjs.getFirst(); item; item = static_cast<GameObjectItem*>(item->getNext()))
    {
        if (item->getObj()->getType() != GAME_OBJ_TYPE_MISSILE)
        {
            LOG_WARN("Collide obj is not missile");
            continue;
        }

        Missile* missile = static_cast<Missile*>(item->getObj());
        missile->explode(gameScreen);
    }
}

} // end of namespace bot
