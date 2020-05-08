#ifndef INLCUDE_BOT_RETURN_CODE
#define INLCUDE_BOT_RETURN_CODE

namespace bot {

enum class ReturnCode {
    OK,
    OUT_OF_SIGHT,           // Object has moved out of sight
    COLLIDE                 // Object has collided with other objects
};

} // end of namespace bot

#endif
