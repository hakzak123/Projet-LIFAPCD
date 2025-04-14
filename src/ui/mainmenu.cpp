#include <ui.h>
#include <map.h>
#include <map>

extern SMM* app;
extern std::map<std::string,SDL_Texture*> globalTextures;
extern map g_map;

void ofdCallback(void *userdata, const char * const *filelist, int filter){

}

void mainMenuSetup(SMM* _app){
    SDL_Texture* textTexture = createTTFTexture(_app->getRenderer(),_app->getFont("impact.ttf"),"TEST TEST TEST",{0,255,0,255});
    const int& width = _app->getWindowInfo().w();
    const int& height = _app->getWindowInfo().h();
    screen* mainMenu = new screen;

    uiButtonRectText* uiNew = new uiButtonRectText(
        _app,
        [](uiButton*){
            app->getUi()["mainMenu"]->setEnabled(false);
            app->getUi()["editor"]->setEnabled(true);
        },
        fRect(width/20,height/4,200,90),
        color(255,255,255,255),
        "New",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    uiButtonRectText* uiEdit = new uiButtonRectText(
        _app,
        [](uiButton*){
            SDL_ShowOpenFileDialog(ofdCallback,nullptr,app->getWindow(),NULL,0,NULL,false);
            app->getUi()["mainMenu"]->setEnabled(false);
            app->getUi()["editor"]->setEnabled(true);
        },
        fRect(width/20,height/2.8,200,90),
        color(255,255,255,255),
        "Edit",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    float spacing = (height / 2.8) - (height / 4);

    uiButtonRectText* uiTestMap = new uiButtonRectText(
        _app,
        nullptr,
        fRect(width/20, (height/2.8) + spacing, 200, 90),
        color(255,255,255,255),
        "Test",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    uiButtonRectTexture* uiSettings = new uiButtonRectTexture(
        [](uiButton*){
            app->getUi()["mainMenu"]->setEnabled(false);
            app->getUi()["settings"]->setEnabled(true);
        },
        fRect(width/20, (height/2.8) + 2*spacing, 200, 90),
        color(255,255,255,255),
        globalTextures["settings_icon.bmp"],
        fRect(0,0,100,100)
    );

    uiButtonRectText* uiQuit = new uiButtonRectText(
        _app,
        [](uiButton*){
            app->appRunning = false;
        },
        fRect(width/20, (height/2.8) + 3*spacing, 200, 90),
        color(255,255,255,255),
        "Quit",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    (*mainMenu)["New"] = uiNew;
    (*mainMenu)["Edit"] = uiEdit;
    (*mainMenu)["Test"] = uiTestMap;
    (*mainMenu)["Settings"] = uiSettings;
    (*mainMenu)["Quit"] = uiQuit;

    _app->insertScreen("mainMenu", mainMenu);
}