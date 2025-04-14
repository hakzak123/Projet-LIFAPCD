#include <ui.h>
#include <map>

extern SMM* app;
extern std::map<std::string,SDL_Texture*> globalTextures;

void settingsSetup(SMM* _app){
    SDL_Texture* textTexture = createTTFTexture(_app->getRenderer(),_app->getFont("impact.ttf"),"TEST TEST TEST",{0,255,0,255});
    const int& width = _app->getWindowInfo().w();
    const int& height = _app->getWindowInfo().h();
    screen* settings = new screen;

    uiButtonRectText* uiMaxFps = new uiButtonRectText(
        _app,
        [](uiButton*){
            unsigned short newFPS;
            bool fail;
            do{
            std::cout << "Enter a new maximum framerate (max : 500)\n";
            std::cin >> newFPS;

            fail = std::cin.fail();
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            } while(newFPS<1 || newFPS >= 500 || fail);
            app->setMaxFramerate(newFPS);
        },
        fRect(width/2-width/12,height/4,300,90),
        color(255,255,255,255),
        "Max Framerate",
        _app->getFont("impact.ttf"),
        60,
        color(0,0,0,255)
    );

    uiButtonRectText* uiSettingsBack = new uiButtonRectText(
        _app,
        [](uiButton*){
            app->getUi()["mainMenu"]->setEnabled(true);
            app->getUi()["settings"]->setEnabled(false);
        },
        fRect(width/20,height/4,200,90),
        color(255,255,255,255),
        "Back",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    (*settings)["MaxFps"] = uiMaxFps;
    (*settings)["Back"] = uiSettingsBack;

    _app->insertScreen("settings", settings);
    _app->getUi()["settings"]->setEnabled(false);
    

}