#ifndef INCLUDE_BOT_GAME_OBJECT_FLAG
#define INCLUDE_BOT_GAME_OBJECT_FLAG

namespace bot {

enum GameObjectFlag {
    GAME_OBJ_FLAG_DRAWN             = 0x00000001,
    GAME_OBJ_FLAG_UPDATED           = 0x00000002,
    GAME_OBJ_FLAG_CHECKED           = 0x00000004,
    GAME_OBJ_FLAG_DEAD              = 0x00000008,
    GAME_OBJ_FLAG_COLLIDABLE        = 0x00000010,
    GAME_OBJ_FLAG_INDESTRUCTABLE    = 0x00000020,
    GAME_OBJ_FLAG_INVISIBLE         = 0x00000040,
    GAME_OBJ_FLAG_PASSTHROUGH       = 0x00000080,
    GAME_OBJ_FLAG_EXPLODE_CHECKED   = 0x00000100
};

} // end of namespace bot

#endif
