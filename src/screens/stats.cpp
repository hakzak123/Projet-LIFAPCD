#include <ui.h>
#include <map>

extern SMM* app;

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

void statsSetup(SMM* _app){
    SDL_Texture* textTexture = createTTFTexture(_app->getRenderer(),_app->getFont("impact.ttf"),"TEST TEST TEST",{0,255,0,255});
    const int& width = _app->getWindowInfo().w();
    const int& height = _app->getWindowInfo().h();
    screen* stats = new screen;

    uiDynamicTextComponent* uiFPS = new uiDynamicTextComponent(
        FPSCount,
        pos(width-width/10,height/14),
        FPSColor
    );

    uiDynamicTextComponent* uiWinInfo = new uiDynamicTextComponent(
        winInfo,
        pos(width-width/10,height/16),
        {48,255,0,255}
    );

    (*stats)["FPS"] = uiFPS;
    (*stats)["WinInfo"] = uiWinInfo;

    _app->insertScreen("stats", stats);
    stats->setEnabled(false);
}