#ifndef INCLUDE_BOT_INPUTMANAGER
#define INCLUDE_BOT_INPUTMANAGER

#include <functional>
#include "bot_opengl.h"
#include "bot_inputevent.h"
#include "bot_queue.h"

namespace bot {

// Prototype of input event processor
// The processor returns:
// 0, proceed to next input;
// 1, clear the event queue and stop processing
// 2, clear the event queue and exit app
typedef std::function<int(const InputEvent&)> InputProcessor;

class InputManager {
public:
    static InputManager g_inputMgr;

    InputManager();

    virtual ~InputManager();

    void init(GLFWwindow *window);

    void start();

    void stop();

    void clear();

    void addMouseButtonEvent(float x, float y, int button, int action);

    void addMouseMoveEvent(float x, float y);

    void addKeyEvent(int key, int action);

    // Returns true if app should continue; false if app should exit
    bool processInput(InputProcessor &processor);

protected:
    GLFWwindow *m_window;
    Queue<InputEvent> m_inputEvents;
};

} // end of namespace bot

#endif

