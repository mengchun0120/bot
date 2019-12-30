#ifndef INCLUDE_BOT_APP
#define INCLUDE_BOT_APP

#include <string>
#include "bot_timedeltasmoother.h"
#include "bot_screenmanager.h"
#include "bot_simpleshaderprogram.h"
#include "bot_textsystem.h"
#include "bot_gamelib.h"

namespace bot {

class App {
public:
    static App g_app;

    App();

    ~App();

    bool run();

    bool init(const char *resDir);

    SimpleShaderProgram& program()
    {
        return m_program;
    }

    TextSystem& textSystem()
    {
        return m_textSystem;
    }

    float viewportWidth() const
    {
        return m_viewportWidth;
    }

    float viewportHeight() const
    {
        return m_viewportHeight;
    }

    const std::string& getResourceDir() const
    {
        return m_resourceDir;
    }

    ScreenManager& screenMgr()
    {
        return m_screenMgr;
    }

    GameLib& gameLib()
    {
        return m_gameLib;
    }

private:

    bool initWindow();

    bool initOpenGL();

    void updateViewport();

    bool initGame();

private:
    GLFWwindow* m_window;
    float m_viewportWidth;
    float m_viewportHeight;
    std::string m_resourceDir;
    TimeDeltaSmoother m_deltaSmoother;
    ScreenManager m_screenMgr;
    SimpleShaderProgram m_program;
    TextSystem m_textSystem;
    GameLib m_gameLib;
};

} // end of namespace bot

#endif

