#include <ui.h>
#include <map>
#include <map.h>

extern SMM* app;
extern std::map<std::string,SDL_Texture*> globalTextures;
extern map g_map;


static bool statsEnabled = false;

void testSetup(SMM* _app){
    SDL_Texture* textTexture = createTTFTexture(_app->getRenderer(),_app->getFont("impact.ttf"),"TEST TEST TEST",{0,255,0,255});
    const int& width = _app->getWindowInfo().w();
    const int& height = _app->getWindowInfo().h();
    screen* test = new screen;

    uiButtonRectText* uiLeave = new uiButtonRectText(
        _app,
        [](uiButton*){
            app->getUi()["test"]->setEnabled(false);
            app->getUi()["mainMenu"]->setEnabled(true);
            g_map.setRenderTarget(fRect(0,0,0,0));
        },
        fRect(0, height-90, 200, 90),
        color(255,255,255,255),
        "Leave",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    (*test)["leave"] = uiLeave;
    _app->insertScreen("test", test);
    _app->getUi()["test"]->setEnabled(false);

}