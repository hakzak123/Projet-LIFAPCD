#include <ui.h>
#include <map.h>
#include <map>
#include <editorInfo.h>

extern SMM* app;
extern std::map<std::string,SDL_Texture*> globalTextures;
extern map g_map;

/*
smp format :
spawnpoint
tilesize
tileMap : uint32 width, height, array of tiles (map components + null terminated strings)
*/

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

        memcpy(file+offset,e.getTextureName().c_str(),e.getTextureName().size()+1);
        offset += e.getTextureName().size()+1;
    }

    SDL_SaveFile(filePath,file,sizeOfFile);

    printf("Saved map to %s\n", filePath);

    return 0;
}

void sfdCallback(void *userdata, const char * const *filelist, int filter){
    if(filelist[0]){
        std::string dest = std::string(filelist[0]) + ".smp";
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

    uiButtonRectTexture* tileButton;
    uiButtonRectText* tileName;

    uiButtonRect* background = new uiButtonRect(nullptr, fRect(0, 0, listWidth, listHeight), color(255,255,255,255), false, false);

    uiButtonRectText* uiSave = new uiButtonRectText(
        _app,
        [](uiButton*){
            SDL_DialogFileFilter filter = {"SMP file", "smp"};
            SDL_ShowSaveFileDialog(sfdCallback, NULL, app->getWindow(), &filter, 1, NULL);
        },
        fRect(0, height-90, 200, 90),
        color(255,255,255,255),
        "Save",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    uiButtonRectText* uiLeave = new uiButtonRectText(
        _app,
        [](uiButton*){
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
        },
        fRect(200 + 30, height-90, 200, 90),
        color(255,255,255,255),
        "Leave",
        _app->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    for(const auto& e : globalTextures){
        tileButton = new uiButtonRectTexture(
            nullptr,
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
    (*editor)["leave"] = uiLeave;

    _app->insertScreen("editor", editor);
    _app->getUi()["editor"]->setEnabled(false);
}