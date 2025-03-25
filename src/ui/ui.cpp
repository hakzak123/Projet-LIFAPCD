#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <unordered_map>
#include <color.h>
#include <ui.h>
#include <windowInfo.h>
#include <smartptr.h>
#include <ttf.h>
#include <application.h>

extern SMM* app;
extern std::unordered_map<std::string,SDL_Texture*> globalTextures;
ui Ui;

// Returns true when at least ms amount of time has passed since the last call. lastTimeStatic MUST BE A ZERO-INITIALIZED STATIC VARIABLE.
bool time_passed_since_last_time(Uint64 ms, Uint64 &lastTimeStatic){
    Uint64 timeNow = SDL_GetTicks();
    
    if(timeNow-lastTimeStatic>=ms){
        lastTimeStatic = timeNow;
        return true;
    }
    return false;
}

// Returns true when at least ms amount of time has passed since lastTimeStatic. lastTimeStatic MUST BE A ZERO-INITIALIZED STATIC VARIABLE. If you want the first call to be true, pass into firstCallStatic a static bool initialized to true.
bool time_passed_since_last_time(Uint64 ms, Uint64 &lastTimeStatic, bool & firstCallStatic){
    Uint64 timeNow = SDL_GetTicks();
    
    if(timeNow-lastTimeStatic>=ms || firstCallStatic){
        lastTimeStatic = timeNow;
        firstCallStatic = false;
        return true;
    }
    return false;
}

std::string FPSCount(){
    static int shownvalue = int(app->getFramerate());
    static Uint64 lastTime = 0;
    static bool firstCall = true;

    if(time_passed_since_last_time(250,lastTime,firstCall)){
        shownvalue = int(app->getFramerate());
    }

    return std::to_string(shownvalue) + " FPS";
}

color FPSColor(){
    static Uint32 count = 0;
    static int shownvalue = static_cast<int>(app->getFramerate());

    if(count < 30){
        count++;
    }
    else{
        count = 0;
        shownvalue = static_cast<int>(app->getFramerate());
    }

    return shownvalue>30 ? color(32,255,48,255) : color(255,32,48,255);
}

std::string winInfo(){
    return std::to_string(app->getWindowInfo().w()) + "x" +std::to_string(app->getWindowInfo().h());
}

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

void uiSetup(){
    Ui = app;
    const int width = app->getWindowInfo().w();
    const int height = app->getWindowInfo().h();
    SDL_Texture* textTexture = createTTFTexture(app->getRenderer(),app->getFont("impact.ttf"),"TEST TEST TEST",{0,255,0,255});

    uiTextureComponent* uiTestTexture = new uiTextureComponent(
        textTexture,
        fRect(width-width/10,height/6,600,110)
    );

    uiTTFComponent* uiTestTTF = new uiTTFComponent(
        app,
        "TEST",
        pos(100,100),
        100,
        app->getFont("impact.ttf"),
        color(0,255,0,255)
    );

    uiTextureComponent* uiTestGlobalTexture = new uiTextureComponent(
        globalTextures["placeholder.bmp"],
        fRect(660,240,600,600)
    );

    uiButtonRect* uiTestButton = new uiButtonRect(
        onClickTest,
        fRect(width-width/10,height/3,600,100),
        color(0,255,0,255)
    );

    uiButtonRectText* uiTestTextButton = new uiButtonRectText(
        app,
        onClickDebug,
        fRect(width-width/10,height/2,300,100),
        color(255,255,255,255),
        "TESTBTN",
        app->getFont("impact.ttf"),
        65,
        color(0,0,0,255)

    );

    uiButtonRectTexture* uiTestTexureButton = new uiButtonRectTexture(
        [](uiButton*){
            SDL_ShowOpenFileDialog(openFileDialog,nullptr,app->getWindow(),NULL,0,NULL,false);
        },
        fRect(width-width/10,height/1.5,300,100),
        color(255,255,255,255),
        globalTextures["placeholder.bmp"],
        fRect(0,0,100,100)
    );

    uiTextComponent* uiTest = new uiTextComponent(
        "test test test test te",
        pos(width-width/10,height/8),
        {48,255,0,255}
    );

    uiDynamicTextComponent* uiFPS = new uiDynamicTextComponent(
        FPSCount,
        pos(width-width/10,height/10),
        FPSColor
    );

    uiDynamicTextComponent* uiWinInfo = new uiDynamicTextComponent(
        winInfo,
        pos(width-width/10,height/12),
        {48,255,0,255}
    );

    Ui["Test"] = uiTest;
    Ui["FPS"] = uiFPS;
    Ui["textureTest"] = uiTestTexture;
    Ui["ttfTest"] = uiTestTTF;
    Ui["testGlobalTexture"] = uiTestGlobalTexture;
    Ui["testButton"] = uiTestButton;
    Ui["testButtonText"] = uiTestTextButton;
    Ui["testButtonTexture"] = uiTestTexureButton;
    Ui["WinInfo"] = uiWinInfo; 

}
