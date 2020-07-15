#ifndef INCLUDE_BOT_APP
#define INCLUDE_BOT_APP

#include <string>
#include <memory>
#include "input/bot_input_manager.h"
#include "opengl/bot_simple_shader_program.h"
#include "opengl/bot_particle_shader_program.h"
#include "opengl/bot_text_system.h"
#include "gameutil/bot_time_delta_smoother.h"
#include "gameutil/bot_game_lib.h"
#include "screen/bot_screen_manager.h"
#include "app/bot_app_config.h"

struct GLFWwindow;

namespace bot {

class App {
public:
    static bool initInstance(const std::string& appDir, const std::string& cfgFile);

    static App& getInstance()
    {
        return *k_app;
    }

    ~App();

    const AppConfig& getConfig() const
    {
        return m_config;
    }

    bool run();

    float getViewportWidth() const
    {
        return m_viewportSize[0];
    }

    float getViewportHeight() const
    {
        return m_viewportSize[1];
    }
    
    const float* getViewportSize() const
    {
        return m_viewportSize;
    }

    SimpleShaderProgram& getSimpleShaderProgram()
    {
        return m_simpleShaderProgram;
    }

    ParticleShaderProgram& getParticleShaderProgram()
    {
        return m_particleShaderProgram;
    }

    const GameLib& getGameLib() const
    {
        return m_gameLib;
    }

    const TextSystem& getTextSystem() const
    {
        return m_textSystem;
    }

    ScreenManager& getScreenManager()
    {
        return m_screenMgr;
    }

private:
    App();

    bool init(const std::string& appDir, const std::string& cfgFile);

    bool initWindow();

    bool initInputManager();

    bool initOpenGL();

    void updateViewport();

    bool initGame();

    bool initTimeDeltaSmoother();

    bool initTextSystem();

    bool initGameLib();

private:
    static std::shared_ptr<App> k_app;

    GLFWwindow* m_window;
    AppConfig m_config;
    float m_viewportSize[Constants::NUM_FLOATS_PER_POSITION];
    InputManager m_inputMgr;
    SimpleShaderProgram m_simpleShaderProgram;
    ParticleShaderProgram m_particleShaderProgram;
    TimeDeltaSmoother m_timeDeltaSmoother;
    TextSystem m_textSystem;
    GameLib m_gameLib;
    ScreenManager m_screenMgr;
};

} // end of namespace bot

#endif

