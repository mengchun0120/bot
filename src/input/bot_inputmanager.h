#ifndef INCLUDE_BOT_INPUTMANAGER
#define INCLUDE_BOT_INPUTMANAGER

#include <functional>
#include "structure/bot_queue.h"
#include "opengl/bot_opengl.h"
#include "input/bot_inputevent.h"

namespace bot {

// Prototype of input event processor
// The processor returns:
// 0, proceed to next input;
// 1, clear the event queue and stop processing
// 2, clear the event queue and exit app
typedef std::function<int(const InputEvent&)> InputProcessor;

class InputManager {
public:
    InputManager();

    virtual ~InputManager();

    void init(GLFWwindow* window, int eventQueueSize, float viewPortHeight);

    void start();

    void stop();

    void clear();

    void addMouseButtonEvent(float x, float y, int button, int action);

    void addMouseMoveEvent(float x, float y);

    void addKeyEvent(int key, int action);

    // Returns true if app should continue; false if app should exit
    bool processInput(InputProcessor& processor);

protected:
    GLFWwindow *m_window;
    Queue<InputEvent> m_inputEvents;
    float m_viewportHeight;
};

} // end of namespace bot

#endif
