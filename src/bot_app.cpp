#include "bot_log.h"
#include "bot_config.h"
#include "bot_inputmanager.h"
#include "bot_utils.h"
#include "bot_app.h"

namespace bot {

App App::g_app;

App::App()
    : m_window(nullptr)
    , m_viewportWidth(0.0f)
    , m_viewportHeight(0.0f)
{
}

App::~App()
{
    if(!m_window) {
        glfwTerminate();
    }
}

bool App::init(const char *appDir)
{
    m_appDir = appDir;
    m_resourceDir = constructPath({appDir, "res"});
    m_saveDir = constructPath({ appDir, "save" });

    if(!initWindow()) {
        LOG_ERROR("Failed to initialize window");
        return false;
    }

    if(!initOpenGL()) {
        LOG_ERROR("Failed to initialize OpenGL");
        return false;
    }

    if(!initGame()) {
        LOG_ERROR("Failed to initialize game");
        return false;
    }

    return true;
}

bool App::run()
{
    float delta;
    int ret;
    InputManager& inputMgr = InputManager::g_inputMgr;
    InputProcessor processor = std::bind(&ScreenManager::processInput,
        &m_screenMgr,
        std::placeholders::_1);

    m_deltaSmoother.start();    
    inputMgr.start();

    while(glfwWindowShouldClose(m_window) == 0) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (!inputMgr.processInput(processor)) {
            break;
        }

        delta = m_deltaSmoother.getTimeDelta();
        ret = m_screenMgr.update(delta);
        if (ret == 2) {
            // the app should exit
            break;
        }
        else if (ret == 1) {
            // switched to another screen, clear input
            inputMgr.clear();
        }

        m_screenMgr.present();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    return true;
}

bool App::initWindow()
{
    LOG_INFO("Initializing window");

    if(!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Config &cfg = Config::g_cfg;

    m_window = glfwCreateWindow(cfg.m_width, cfg.m_height, cfg.m_title.c_str(),
                                NULL, NULL);
    if(!m_window){
        LOG_ERROR("Failed to open GLFW window");
        return false;
    }

    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        LOG_ERROR("Failed to initialize GLEW");
        return false;
    }

    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
    
    InputManager::g_inputMgr.init(m_window);

    return true;
}

bool App::initOpenGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!m_program.init()) {
        LOG_ERROR("Failed to init shader program");
        return false;
    }

    m_program.use();

    updateViewport();
    return true;
}

void App::updateViewport()
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    m_viewportWidth = static_cast<float>(width);
    m_viewportHeight = static_cast<float>(height);

    float viewportSize[] = { m_viewportWidth, m_viewportHeight };

    m_program.setViewportSize(viewportSize);
}

bool App::initGame()
{
    m_deltaSmoother.init();
    m_textSystem.init();
    m_screenMgr.init();
    if (!m_gameLib.init()) {
        LOG_ERROR("Failed to initialize game lib");
        return false;
    }
    return true;
}

} // end of namespace bot

