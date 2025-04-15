#include <ui.h>
#include <map.h>
#include <map>

extern SMM* app;
extern std::map<std::string,SDL_Texture*> globalTextures;
extern map g_map;

Uint32 loadMapEvent = 0;

/*
smp format :
spawnpoint
tilesize
tileMap : uint32 width, height, array of tiles (map components + null terminated strings)
*/

int loadMapFromFile(const char* filePath){ // ajouter des checks
    size_t fileSize;
    size_t addition = 0;

    if(filePath == nullptr){
        return -1;
    }

    char* file = (char*)SDL_LoadFile(filePath,&fileSize);
    
    spawnPoint spawn = *(spawnPoint*)file;
    addition += sizeof(spawnPoint);
    g_map.setSpawnPoint(spawn);
    
    unsigned tileSize = *(unsigned*)(file + addition);
    addition += sizeof(unsigned);
    g_map.setTileSize(tileSize);
    
    unsigned width, height;
    width = *(unsigned*)(file + addition);
    addition += sizeof(unsigned);
    height = *(unsigned*)(file + addition);
    g_map.getTileMap().newDim(width,height);

    int index = 0;
    while(addition <= fileSize){
        mapComponent m = *(mapComponent*)(file + addition);
        addition += sizeof(mapComponent);
        std::string s = (const char*)(file + addition);
        addition += s.size() + 1;

        tile t(s,m.getPos(),m.getCollision());
        g_map.getTileMap().getTiles()[index] = t;
        index++;
    }

    SDL_free(file);
    return 0;
}

void ofdCallback(void *userdata, const char * const *filelist, int filter){
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
            app->getUi()["mainMenu"]->setEnabled(false);
            app->getUi()["editor"]->setEnabled(true);
            g_map.setFilePath(filelist[0]); // remettre à 0 dans le bouton discard
            event.user.code = 0;
        }
        case -1 : {
            event.user.code = -1;
        }
    }

    SDL_PushEvent(&event);
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