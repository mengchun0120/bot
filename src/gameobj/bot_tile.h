#ifndef INCLUDE_BOT_TILE
#define INCLUDE_BOT_TILE

#include "misc/bot_constants.h"
#include "gameobj/bot_gameobject.h"

namespace bot {

class TileTemplate;

class Tile : public GameObject {
public:
    Tile(const TileTemplate* tileTemplate);

    virtual ~Tile();

    virtual void present(SimpleShaderProgram& program);

    virtual bool update(float delta, GameScreen& screen);

    virtual float getPosX() const
    {
        return m_pos[0];
    }

    virtual float getPosY() const
    {
        return m_pos[1];
    }

    virtual void setPos(float x, float y)
    {
        m_pos[0] = x;
        m_pos[1] = y;
    }

    virtual float getCoverBreathX() const;

    virtual float getCoverBreathY() const;

    virtual float getCollideBreathX() const;

    virtual float getCollideBreathY() const;

protected:
    const TileTemplate* m_tileTemplate;
    float m_pos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif
