#include <ui.h>
#include <map.h>
#include <map>
#include <editorInfo.h>

extern SMM* app;
extern map g_map;

Uint32 loadMapEvent = 0;
static const char* prevNext[2];

int loadMapFromFile(const char* filePath);

void ofdCallback(void *userdata, const char * const *filelist, int filter){ // (const char*)userdata = next screen, (const char*)userdata+1 = previous screen
    const char** converted = (const char**)userdata;
    std::string prev = converted[0];
    std::string next = converted[1];

    if(loadMapEvent == 0)
        loadMapEvent = SDL_RegisterEvents(1);

    SDL_Event event;
    event.type = loadMapEvent;
    event.user.timestamp = SDL_GetTicksNS();
    event.user.data1 = nullptr;
    event.user.data2 = nullptr;

    int errorCode = loadMapFromFile(filelist[0]);

    switch(errorCode){
        case 0 : {
            app->getUi()[next]->setEnabled(true);
            g_map.setFilePath(filelist[0]); // remettre à 0 dans le bouton discard
            g_map.init();
            if(next == "editor"){
                g_map.setRenderTarget(fRect(listWidth, 0, app->getWindowInfo().w() - listWidth, listHeight));
                g_map.setTestMode(false);
            }
            else if(next == "test"){
                g_map.setRenderTarget(fRect(0, 0, app->getWindowInfo().w(), app->getWindowInfo().h()));
                g_map.setTestMode(true);
            }
            g_map.setRendered(true);
            event.user.code = 0;
            break;
        }
        case -1 : {
            app->getUi()[prev]->setEnabled(true);
            if(prev != "editor")
                g_map.setRendered(false);
            else
                g_map.setRendered(true);
            event.user.code = -1;
            break;
        }
    }

    app->getUi()["loading"]->setEnabled(false);

    SDL_PushEvent(&event);
}

void mainMenuSetup(SMM* _app){
    SDL_Texture* textTexture = createTTFTexture(_app->getRenderer(),_app->getFont("impact.ttf"),"TEST TEST TEST",{0,255,0,255});
    const int& width = _app->getWindowInfo().w();
    const int& height = _app->getWindowInfo().h();
    screen* mainMenu = new screen;
    float spacing = (height / 2.8) - (height / 4);

    uiButtonRectTextureScaled* uiLogo= new uiButtonRectTextureScaled(
        nullptr,
        fRect(width/2-width/8.2,height/1000,500,500),
        color(255,255,255,0),
        app->getGlobalTextures()["logo.bmp"],
        fRect(0,0,500,500),
        true,
        false
    );


    uiButtonRectText* uiNew = new uiButtonRectText(
        _app,
        [](uiButton*){
            g_map = map(app, tileMap(), spawnPoint());
            g_map.init();
            g_map.setRenderTarget(fRect(listWidth, 0, app->getWindowInfo().w() - listWidth, listHeight));
            app->getUi()["mainMenu"]->setEnabled(false);
            app->getUi()["editor"]->setEnabled(true);
            g_map.setRendered(true);
        },
        fRect(width/20,height/2.8,200,90),
        color(255,255,255,255),
        "New",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    uiButtonRectText* uiEdit = new uiButtonRectText(
        _app,
        [](uiButton*){
            SDL_DialogFileFilter filter = {"SMP file", "smp"};

            prevNext[0] = "mainMenu";
            prevNext[1] = "editor";

            SDL_ShowOpenFileDialog(ofdCallback,(void*)prevNext,app->getWindow(),&filter,1,NULL,false);
            app->getUi()["mainMenu"]->setEnabled(false);
            app->getUi()["loading"]->setEnabled(true);
        },
        fRect(width/20, (height/2.8) + spacing,200,90),
        color(255,255,255,255),
        "Edit",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );



    uiButtonRectText* uiTestMap = new uiButtonRectText(
        _app,
        [](uiButton*){
            SDL_DialogFileFilter filter = {"SMP file", "smp"};

            prevNext[0] = "mainMenu";
            prevNext[1] = "test";

            SDL_ShowOpenFileDialog(ofdCallback, (void*)prevNext,app->getWindow(),&filter,1,NULL,false);
            app->getUi()["mainMenu"]->setEnabled(false);
            app->getUi()["loading"]->setEnabled(true);
            g_map.getPlayer().setPos(g_map.getSpawnPoint().getPos());
        },
        fRect(width/20, (height/2.8) + spacing*2, 200, 90),
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
        fRect(width/20, (height/2.8) + 3*spacing, 200, 90),
        color(255,255,255,255),
        app->getGlobalTextures()["settings_icon.bmp"],
        fRect(0,0,100,100)
    );

    uiButtonRectText* uiQuit = new uiButtonRectText(
        _app,
        [](uiButton*){
            app->appRunning = false;
        },
        fRect(width/20, (height/2.8) + 4*spacing, 200, 90),
        color(255,255,255,255),
        "Quit",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    (*mainMenu)["Logo"] = uiLogo;
    (*mainMenu)["New"] = uiNew;
    (*mainMenu)["Edit"] = uiEdit;
    (*mainMenu)["Test"] = uiTestMap;
    (*mainMenu)["Settings"] = uiSettings;
    (*mainMenu)["Quit"] = uiQuit;

    _app->insertScreen("mainMenu", mainMenu);
}