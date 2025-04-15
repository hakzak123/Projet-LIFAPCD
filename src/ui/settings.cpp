#include <ui.h>
#include <map>
#include <tinyfiledialogs.h>

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
            unsigned short newFPS = -1;

            do{
                char* input = tinyfd_inputBox("Enter a new framerate", "Integer between 1 and 500", "60");
                if(!input){
                    continue;
                }
                try{
                    newFPS = std::stoi(input);
                }
                catch(const std::invalid_argument&){
                    continue;
                }
                catch(const std::out_of_range&){
                    continue;
                }
            } while(newFPS<1 || newFPS >= 500);

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