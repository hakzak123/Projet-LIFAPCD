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
    static int shownvalue = int(app->framerate);
    static Uint64 lastTime = 0;
    static bool firstCall = true;

    if(time_passed_since_last_time(250,lastTime,firstCall)){
        shownvalue = int(app->framerate);
    }

    return std::to_string(shownvalue) + " FPS";
}

color FPSColor(){
    static Uint32 count = 0;
    static int shownvalue = static_cast<int>(app->framerate);

    if(count < 30){
        count++;
    }
    else{
        count = 0;
        shownvalue = static_cast<int>(app->framerate);
    }

    return shownvalue>30 ? color(32,255,48,255) : color(255,32,48,255);
}

std::string winInfo(){
    return std::to_string(app->winfo.w()) + "x" +std::to_string(app->winfo.h());
}

void onClickTest(uiButton* parent){
    ((uiButtonRect*)parent)->setRectColor({(Uint8)SDL_rand(254),(Uint8)SDL_rand(254),(Uint8)SDL_rand(254),255});
}

void uiSetup(){
    int width = app->winfo.w();
    int height = app->winfo.h();
    SDL_Texture* textTexture = createTTFTexture(app->renderer,app->fontMap["impact.ttf"],"TEST TEST TEST",{0,255,0,255});

    uiTextureComponent* uiTestTexture = new uiTextureComponent(
        app,
        textTexture,
        fRect(width-width/10,height/6,600,120)
    );

    uiTextureComponent* uiTestGlobalTexture = new uiTextureComponent(
        app,
        globalTextures["placeholder.bmp"],
        fRect(660,240,600,600)
    );

    uiButtonRect* uiTestButton = new uiButtonRect(
        app,
        onClickTest,
        fRect(width-width/10,height/3,600,100),
        color(0,255,0,255)
    );

    uiTextComponent* uiTest = new uiTextComponent(
        app,
        "test test test test te",
        pos(width-width/10,height/8),
        {48,255,0,255}
    );

    uiDynamicTextComponent* uiFPS = new uiDynamicTextComponent(
        app,
        FPSCount,
        pos(width-width/10,height/10),
        FPSColor
    );

    uiDynamicTextComponent* uiWinInfo = new uiDynamicTextComponent(
        app,
        winInfo,
        pos(width-width/10,height/12),
        {48,255,0,255}
    );

    Ui.insert("Test",uiTest);
    Ui.insert("FPS",uiFPS);
    Ui.insert("ttfTest",uiTestTexture);
    Ui.insert("testGlobalTexture",uiTestGlobalTexture);
    Ui.insert("testButton",uiTestButton);
    Ui.insert("WinInfo",uiWinInfo); 

}
