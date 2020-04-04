#ifndef INCLUDE_BOT_APP
#define INCLUDE_BOT_APP

#include <string>
#include <rapidjson/document.h>
#include "input/bot_inputmanager.h"
#include "opengl/bot_simpleshaderprogram.h"
#include "opengl/bot_textsystem.h"
#include "gameutil/bot_timedeltasmoother.h"
#include "gametemplate/bot_gametemplatelib.h"

struct GLFWwindow;

namespace bot {

class App {
public:
    App();

    ~App();

    bool init(const std::string& appDir, const std::string& cfgFile);

    bool run();

    float viewportWidth() const
    {
        return m_viewportWidth;
    }

    float viewportHeight() const
    {
        return m_viewportHeight;
    }

private:
    bool initWindow(const rapidjson::Value& cfg);

    bool initInputManager(const rapidjson::Value& cfg);

    bool initOpenGL(const rapidjson::Value& cfg);

    void updateViewport();

    bool initGame(const rapidjson::Value& cfg);

    bool initTimeDeltaSmoother(const rapidjson::Value& cfg);

    bool initTextSystem(const rapidjson::Value& cfg);

    bool initGameTemplateLib(const rapidjson::Value& cfg);

private:
    GLFWwindow* m_window;
    int m_timeDeltaHistoryLen;
    int m_eventQueueSize;
    std::string m_appDir;
    std::string m_resDir;
    float m_viewportWidth;
    float m_viewportHeight;
    InputManager m_inputMgr;
    SimpleShaderProgram m_program;
    TimeDeltaSmoother m_timeDeltaSmoother;
    TextSystem m_textSystem;
    GameTemplateLib m_gameTemplateLib;
};

} // end of namespace bot

#endif

