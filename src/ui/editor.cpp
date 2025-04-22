#include <ui.h>
#include <map.h>
#include <map>
#include <editorInfo.h>
#include <tinyfiledialogs.h>

extern SMM* app;
extern std::map<std::string,SDL_Texture*> globalTextures;
extern map g_map;

static SDL_Texture* selectedTexture = globalTextures["placeholder.bmp"];
static bool collisions = true;

int loadMapFromFile(const char*);
void ofdCallback(void *, const char * const *, int);

/*
smp format :
spawnpoint
tilesize
tileMap : uint32 width, height, array of tiles (map components + null terminated strings)
*/

void LeaveWarning(uiButton*){
    SDL_MessageBoxButtonData buttons[2];
    SDL_MessageBoxButtonData cancel;
    SDL_MessageBoxButtonData understand;
    SDL_MessageBoxData warningBoxData;
    int buttonId;

    cancel.buttonID = 1;
    cancel.text = "Cancel";

    understand.buttonID = 0;
    understand.text = "I understand";

    buttons[0] = cancel;
    buttons[1] = understand;

    warningBoxData.flags = SDL_MESSAGEBOX_WARNING;
    warningBoxData.window = app->getWindow();
    warningBoxData.title = "Warning";
    warningBoxData.message = "Any unsaved changed will be discarded.";
    warningBoxData.numbuttons = 2;
    warningBoxData.buttons = buttons;
    warningBoxData.colorScheme = NULL;

    SDL_ShowMessageBox(&warningBoxData, &buttonId);

    if(buttonId == 0){
        app->getUi()["editor"]->setEnabled(false);
        app->getUi()["mainMenu"]->setEnabled(true);
        g_map.setRenderTarget(fRect(0,0,0,0));
    }

    SDL_FlushEvent(SDL_EVENT_MOUSE_BUTTON_DOWN);
    SDL_FlushEvent(SDL_EVENT_MOUSE_BUTTON_UP);
    SDL_FlushEvent(SDL_EVENT_MOUSE_MOTION);
}

int saveMapToFile(const char* filePath){
    size_t sizeOfFile = 0;
    size_t offset = 0;
    char* file;
    tileMap& tile_map = g_map.getTileMap();

    sizeOfFile += sizeof(spawnPoint);
    sizeOfFile += sizeof(unsigned)*3; // tilesize, width, height
    sizeOfFile += sizeof(mapComponent)*tile_map.getTiles().size();
    
    for(const auto e : tile_map.getTiles()){
        sizeOfFile += e.getTextureName().size() + 1;
    }

    file = (char*)SDL_malloc(sizeOfFile);

    *(spawnPoint*)file = g_map.getSpawnPoint();
    offset += sizeof(spawnPoint);

    *(unsigned*)(file + offset) = g_map.getTileSize();
    offset += sizeof(unsigned);

    *(unsigned*)(file + offset) = tile_map.getWidth();
    offset += sizeof(unsigned);
    *(unsigned*)(file + offset) = tile_map.getHeight();
    offset += sizeof(unsigned);

    for(const auto e : tile_map.getTiles()){
        *(mapComponent*)(file + offset) = e;
        offset += sizeof(mapComponent);

        memcpy(file+offset, e.getTextureName().c_str(), e.getTextureName().size()+1);
        offset += e.getTextureName().size()+1;
    }

    SDL_SaveFile(filePath,file,sizeOfFile);

    printf("Saved map to %s\n", filePath);

    return 0;
}

void sfdCallback(void *userdata, const char * const *filelist, int filter){
    if(filelist[0]){
        std::string dest = std::string(filelist[0]);
        if(dest.substr(dest.size()-4) != ".smp"){
            dest += ".smp";
        }
        saveMapToFile(dest.c_str());
    }
}

void editorSetup(SMM* _app){
    const int& width = _app->getWindowInfo().w();
    const int& height = _app->getWindowInfo().h();
    screen* editor = new screen;
    int index = 0;
    const unsigned listLineSize = 3;
    editorInfo listWidth = editorInfo::listWidth, listHeight = editorInfo::listHeight;
    const unsigned blockSize = listWidth/listLineSize;

    uiButtonRectTextureScaled* tileButton;
    uiButtonRectText* tileName;

    uiButtonRect* background = new uiButtonRect(nullptr, fRect(0, 0, listWidth, listHeight), color(255,255,255,255), false, false);

    uiButtonRectTextureScaled* selectionZone = new uiButtonRectTextureScaled(
        [](uiButton*){
            pos mousePos;
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            tile* t = g_map.getTileFromWinPos(mousePos.x, mousePos.y);
            if(selectedTexture && t && g_map.mapBeginX() <= mousePos.x && g_map.mapBeginY() <= mousePos.y){
                for(const auto& e : globalTextures){
                    if(selectedTexture == e.second)
                        t->setTexture(e.first);
                        t->setCollision(collisions);
                }
            }
        },
        fRect(listWidth, 0, app->getWindowInfo().w() - listWidth, listHeight), 
        color(255,255,255,0), 
        globalTextures["transparent.bmp"], 
        fRect(), 
        true, 
        true, 
        false);

    uiButtonRectText* uiSave = new uiButtonRectText(
        _app,
        [](uiButton*){
            SDL_DialogFileFilter filter = {"SMP file", "smp"};
            SDL_ShowSaveFileDialog(sfdCallback, NULL, app->getWindow(), &filter, 1, NULL);
        },
        fRect(0, height-310, 200, 90),
        color(255,255,255,255),
        "Save",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    uiButtonRectText* uiLoad = new uiButtonRectText(
        _app,
        [](uiButton*){
            SDL_DialogFileFilter filter = {"SMP file", "smp"};
            SDL_ShowOpenFileDialog(ofdCallback,nullptr,app->getWindow(),&filter,1,NULL,false);
            app->getUi()["editor"]->setEnabled(false);
            g_map.setRendered(false);
            app->getUi()["loading"]->setEnabled(true);
        },
        fRect(0, height-200, 200, 90),
        color(255,255,255,255),
        "Load",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    uiButtonRectText* uiLeave = new uiButtonRectText(
        _app,
        LeaveWarning,
        fRect(0, height-90, 200, 90),
        color(255,255,255,255),
        "Leave",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );


    uiButtonRectText* uiCameraSpeed = new uiButtonRectText(
        _app,
        [](uiButton*){
            unsigned short newSpeed = -1;

            do{
                char* input = tinyfd_inputBox("Enter a new speed", "Integer superior to 1", std::to_string(g_map.getCamera().getEditorSpeed()).c_str());
                if(!input){
                    newSpeed = g_map.getCamera().getEditorSpeed();
                    break;
                }
                try{
                    newSpeed = std::stof(input);
                }
                catch(const std::invalid_argument&){
                    continue;
                }
                catch(const std::out_of_range&){
                    continue;
                }
            } while(newSpeed<1);

            g_map.getCamera().setEditorSpeed(newSpeed);

        },
        fRect(width-200, height-90, 200, 90),
        color(255,255,255,255),
        "Speed",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    uiButtonRectText* uiNewDim = new uiButtonRectText(
        _app,
        [](uiButton*){
            unsigned newWidth = 0;
            unsigned newHeight = 0;

            do{
                char* input = tinyfd_inputBox("Enter a new width", "Integer superior or equal to 8", std::to_string(g_map.getTileMap().getWidth()).c_str());
                if(!input){
                    newWidth = g_map.getTileMap().getWidth();
                    break;
                }
                try{
                    newWidth = std::stoul(input);
                }
                catch(const std::invalid_argument&){
                    continue;
                }
                catch(const std::out_of_range&){
                    continue;
                }
            } while(newWidth < 8);

            do{
                char* input = tinyfd_inputBox("Enter a new height", "Integer superior or equal to 8", std::to_string(g_map.getTileMap().getHeight()).c_str());
                if(!input){
                    newHeight = g_map.getTileMap().getHeight();
                    break;
                }
                try{
                    newHeight = std::stoul(input);
                }
                catch(const std::invalid_argument&){
                    continue;
                }
                catch(const std::out_of_range&){
                    continue;
                }
            } while(newHeight < 8);


            g_map.getTileMap().newDim(newWidth, newHeight);
            g_map.getCamera().setPos(pos(0,0));
            g_map.init();
        },
        fRect(width-400-20, height-90, 200, 90),
        color(255,255,255,255),
        "New Dim",
        _app->getFont("impact.ttf"),
        70,
        color(0,0,0,255)
    );

    uiButtonRectText* uiTileSize= new uiButtonRectText(
        _app,
        [](uiButton*){
            unsigned newSize= -1;

            do{
                char* input = tinyfd_inputBox("Enter a new size", "Integer superior to 1", std::to_string(g_map.getTileSize()).c_str());
                if(!input){
                    newSize = g_map.getTileSize();
                    break;
                }
                try{
                    newSize = std::stof(input);
                }
                catch(const std::invalid_argument&){
                    continue;
                }
                catch(const std::out_of_range&){
                    continue;
                }
            } while(newSize < 1);

            g_map.setTileSize(newSize);

        },
        fRect(width-600-40, height-90, 200, 90),
        color(255,255,255,255),
        "Tile Size",
        _app->getFont("impact.ttf"),
        65,
        color(0,0,0,255)
    );

    uiButtonRectText* uiBackToOrigin = new uiButtonRectText(
        _app,
        [](uiButton*){
            g_map.getCamera().setPos(pos(0,0));
        },
        fRect(width-800-60, height-90, 200, 90),
        color(255,255,255,255),
        "Go to origin",
        _app->getFont("impact.ttf"),
        50,
        color(0,0,0,255)
    );

    
    uiButtonRectText* uiSetCollisions = new uiButtonRectText(
        _app,
        [](uiButton* button){
            if(collisions){
                ((uiButtonRect*)button)->setRectColor(color(255,0,0,255));
                collisions = false;
            }
            else{
                ((uiButtonRect*)button)->setRectColor(color(255,255,255,255));
                collisions = true;
            }
        },
        fRect(width-1000-80, height-90, 200, 90),
        color(255,255,255,255),
        "Collisions",
        _app->getFont("impact.ttf"),
        55,
        color(0,0,0,255)
    );


    for(const auto& e : globalTextures){
        tileButton = new uiButtonRectTextureScaled(
            [](uiButton* button){
                selectedTexture = button->getTexture();
            },
            fRect(index % listLineSize * blockSize, index / listLineSize * blockSize, blockSize, blockSize),
            color(255,255,255,0),
            e.second,
            fRect(0,0,blockSize,blockSize),
            false
        );

        tileName = new uiButtonRectText(
            _app,
            nullptr,
            fRect(index % listLineSize * blockSize, index / listLineSize * blockSize, blockSize, blockSize/5),
            color(127,127,127,255),
            e.first,_app->getFont("impact.ttf"),
            blockSize/7,
            color(255,255,255,255),
            true,
            false
        );
        
        (*editor)[e.first] = tileButton;
        (*editor)[e.first + " name"] = tileName;
        index++;
    }

    (*editor)["background"] = background;
    (*editor)["save"] = uiSave;
    (*editor)["load"] = uiLoad;
    (*editor)["leave"] = uiLeave;
    (*editor)["cameraSpeed"] = uiCameraSpeed;
    (*editor)["newDim"] = uiNewDim;
    (*editor)["tileSize"] = uiTileSize;
    (*editor)["backToOrigin"] = uiBackToOrigin;
    (*editor)["selectionZone"] = selectionZone;
    (*editor)["setCollisions"] = uiSetCollisions;


    _app->insertScreen("editor", editor);
    _app->getUi()["editor"]->setEnabled(false);
}