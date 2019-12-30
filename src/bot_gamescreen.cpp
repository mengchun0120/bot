#include "bot_app.h"
#include "bot_gameobject.h"
#include "bot_inputevent.h"
#include "bot_gamescreen.h"

namespace bot {

GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
}

bool GameScreen::init()
{
    GameLib &lib = App::g_app.gameLib();
    m_obj.init(&lib.getGameObjectTemplate(3), 100.0f, 100.0f);
    return true;
}

int GameScreen::update(float delta)
{
    return 0;
}

void GameScreen::present()
{
    m_obj.present();
    float pos[] = {200.0f, 200.0f};
    GameLib &lib = App::g_app.gameLib();
    Texture &tex = lib.getTexture(0);
    Rectangle &rect = lib.getRect(0);
    rect.draw(App::g_app.program(), pos, nullptr, nullptr, nullptr, tex.textureId(), nullptr);
}

int GameScreen::processInput(const InputEvent &e)
{
    return 0;
}


} // end of namespace bot
