#include <ui.h>
#include <map.h>
#include <map>
#include <editorInfo.h>

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
    size_t offset = 0;
    tileMap& tile_map = g_map.getTileMap();

    if(filePath == nullptr){
        return -1;
    }

    char* file = (char*)SDL_LoadFile(filePath,&fileSize);
    
    spawnPoint spawn = *(spawnPoint*)file;
    offset += sizeof(spawnPoint);
    g_map.setSpawnPoint(spawn);
    
    unsigned tileSize = *(unsigned*)(file + offset);
    offset += sizeof(unsigned);
    g_map.setTileSize(tileSize);
    
    unsigned width, height;
    width = *(unsigned*)(file + offset);
    offset += sizeof(unsigned);
    height = *(unsigned*)(file + offset);
    offset += sizeof(unsigned);
    tile_map.newDim(width,height);

    int index = 0;
    tile_map.newDim(width,height);

    while(offset < fileSize){
        mapComponent m = *(mapComponent*)(file + offset);
        offset += sizeof(mapComponent);
        std::string s = (const char*)(file + offset);
        offset += s.size() + 1;

        tile t(s,m.getPos(),m.getCollision());
        tile_map.getTiles()[index] = t;
        index++;
    }

    printf("Map loaded. Map dimensions : %ux%u, amount of tiles : %u\n",tile_map.getWidth(), tile_map.getHeight(), tile_map.getTiles().size());

    SDL_free(file);
    return 0;
}

void ofdCallback(void *userdata, const char * const *filelist, int filter){
    const char* converted = (const char*)userdata;
    std::string screenName = converted;

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
            app->getUi()[screenName]->setEnabled(true);
            g_map.setFilePath(filelist[0]); // remettre à 0 dans le bouton discard
            g_map.init();
            if(screenName == "editor"){
                g_map.setRenderTarget(fRect(listWidth, 0, app->getWindowInfo().w() - listWidth, listHeight));
                g_map.setTestMode(false);
            }
            else if(screenName == "test"){
                g_map.setRenderTarget(fRect(0, 0, app->getWindowInfo().w(), app->getWindowInfo().h()));
                g_map.setTestMode(true);
            }
            g_map.setRendered(true);
            event.user.code = 0;
            break;
        }
        case -1 : {
            app->getUi()["mainMenu"]->setEnabled(true);
            g_map.setRendered(false);
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
        globalTextures["logo.bmp"],
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
            SDL_ShowOpenFileDialog(ofdCallback,(void*)"editor",app->getWindow(),&filter,1,NULL,false);
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
            SDL_ShowOpenFileDialog(ofdCallback, (void*)"test",app->getWindow(),&filter,1,NULL,false);
            app->getUi()["mainMenu"]->setEnabled(false);
            app->getUi()["loading"]->setEnabled(true);
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
        globalTextures["settings_icon.bmp"],
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