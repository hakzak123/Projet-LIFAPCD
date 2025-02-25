#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <color.h>
#include <ui.h>
#include <windowInfo.h>
#include <smartptr.h>
#include <ttf.h>


extern SDL_Window *window;
extern TTF_Font* testFont;
extern windowInfo winfo;
extern double framerate;
extern double maxframerate;
extern Uint64 framecount;

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
    static int shownvalue = int(framerate);
    static Uint64 lastTime = 0;
    static bool firstCall = true;

    if(time_passed_since_last_time(250,lastTime,firstCall)){
        shownvalue = int(framerate);
    }

    return std::to_string(shownvalue) + " FPS";
}

color FPSColor(){
    static Uint32 count = 0;
    static int shownvalue = static_cast<int>(framerate);

    if(count < 30){
        count++;
    }
    else{
        count = 0;
        shownvalue = static_cast<int>(framerate);
    }

    return shownvalue>30 ? color(32,255,48,255) : color(255,32,48,255);
}

std::string winInfo(){
    return std::to_string(winfo.w)+ "x" +std::to_string(winfo.h);
}

void uiSetup(SDL_Renderer* renderer){
    int width = winfo.w;
    int height = winfo.h;
    SDL_Texture* textTexture = createTTFTexture(renderer,testFont,"test test test",{0,255,0,255});

    uiTextureComponent uiTestTexture(
        renderer,
        textTexture,
        &winfo,
        fRect(width-width/10,height/6,600,100)
    );

    uiTextComponent uiTest(
        renderer,
        "test test test test te",
        pos(width-width/10,height/8),
        {48,255,0,255},
        &winfo
    );

    uiDynamicTextComponent uiFPS(
        renderer,
        FPSCount,
        pos(width-width/10,height/10),
        FPSColor,
        &winfo
    );

    uiDynamicTextComponent uiWinInfo(
        renderer,
        winInfo,
        pos(width-width/10,height/12),
        {48,255,0,255},
        &winfo
    );

    Ui.insert("Test",uiTest);
    Ui.insert("FPS",uiFPS);
    Ui.insert("ttfTest",uiTestTexture);
    Ui.insert("WinInfo",uiWinInfo);

}
