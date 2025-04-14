#include <ui.h>
#include <map>

extern SMM* app;
extern std::map<std::string,SDL_Texture*> globalTextures;

void editorSetup(SMM* _app){
    const int& width = _app->getWindowInfo().w();
    const int& height = _app->getWindowInfo().h();
    screen* editor = new screen;
    int index = 0;
    const unsigned listLineSize = 3;
    const unsigned listWidth = 450, listHeight = 750;
    const unsigned blockSize = listWidth/listLineSize;

    uiButtonRectTexture* tileButton;
    uiButtonRectText* tileName;

    uiButtonRect* background = new uiButtonRect(nullptr, fRect(0, 0, listWidth, listHeight), color(255,255,255,255),false);
    (*editor)["background"] = background;

    for(const auto& e : globalTextures){
        tileButton = new uiButtonRectTexture(
            nullptr,
            fRect(index % listLineSize * blockSize, index / listLineSize * blockSize, blockSize, blockSize),
            color(255,255,255,0),
            e.second,
            fRect(0,0,blockSize,blockSize)
        );

        tileName = new uiButtonRectText(
            _app,
            nullptr,
            fRect(index % listLineSize * blockSize, index / listLineSize * blockSize, blockSize, blockSize/5),
            color(127,127,127,255),
            e.first,_app->getFont("impact.ttf"),
            blockSize/7,
            color(255,255,255,255),
            false
        );
        
        (*editor)[e.first] = tileButton;
        (*editor)[e.first + " name"] = tileName;
        index++;
    }

    _app->insertScreen("editor", editor);
    _app->getUi()["editor"]->setEnabled(false);
}