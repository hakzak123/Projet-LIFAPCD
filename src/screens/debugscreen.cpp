#include <ui.h>
#include <map>

extern SMM* app;

void onClickTest(uiButton* parent){
    ((uiButtonRect*)parent)->setRectColor({(Uint8)SDL_rand(254),(Uint8)SDL_rand(254),(Uint8)SDL_rand(254),255});
}

void onClickDebug(uiButton* parent){
    float x,y;
    SDL_GetMouseState(&x,&y);
    std::cout << x << " " << y << '\n';
}

void openFileDialog(void *userdata, const char * const *filelist, int filter){
}

void debugScreenSetup(SMM* _app){
    SDL_Texture* textTexture = createTTFTexture(_app->getRenderer(),_app->getFont("impact.ttf"),"TEST TEST TEST",{0,255,0,255});
    const int& width = _app->getWindowInfo().w();
    const int& height = _app->getWindowInfo().h();
    screen* debug = new screen;

    uiTextureComponent* uiTestTexture = new uiTextureComponent(
        textTexture,
        fRect(width-width/10,height/6,600,110)
    );

    uiTTFComponent* uiTestTTF = new uiTTFComponent(
        _app,
        "TEST",
        pos(100,100),
        100,
        _app->getFont("impact.ttf"),
        color(0,255,0,255)
    );

    uiTextureComponent* uiTestGlobalTexture = new uiTextureComponent(
        _app->getGlobalTextures()["placeholder.bmp"],
        fRect(660,240,600,600)
    );

    uiButtonRect* uiTestButton = new uiButtonRect(
        onClickTest,
        fRect(width-width/10,height/3,600,100),
        color(0,255,0,255)
    );

    uiButtonRectText* uiTestTextButton = new uiButtonRectText(
        _app,
        onClickDebug,
        fRect(width-width/10,height/2,300,100),
        color(255,255,255,255),
        "TESTBTN",
        _app->getFont("impact.ttf"),
        65,
        color(0,0,0,255)

    );

    uiButtonRectTexture* uiTestTexureButton = new uiButtonRectTexture(
        [](uiButton*){
            SDL_ShowOpenFileDialog(openFileDialog,nullptr,app->getWindow(),NULL,0,NULL,false);
        },
        fRect(width-width/10,height/1.5,300,100),
        color(255,255,255,255),
        _app->getGlobalTextures()["placeholder.bmp"],
        fRect(0,0,100,100)
    );

    uiTextComponent* uiTest = new uiTextComponent(
        "test test test test te",
        pos(width-width/10,height/8),
        {48,255,0,255}
    );

    (*debug)["Test"] = uiTest;
    (*debug)["textureTest"] = uiTestTexture;
    (*debug)["ttfTest"] = uiTestTTF;
    (*debug)["testGlobalTexture"] = uiTestGlobalTexture;
    (*debug)["testButton"] = uiTestButton;
    (*debug)["testButtonText"] = uiTestTextButton;
    (*debug)["testButtonTexture"] = uiTestTexureButton;

    _app->insertScreen("debug", debug);
    _app->getUi()["debug"]->setEnabled(false);
}