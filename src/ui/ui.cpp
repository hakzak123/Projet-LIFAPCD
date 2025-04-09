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

void SMM::uiSetup(){
    screen* mainMenu = new screen; // à delete
    screen* debug = new screen;
    screen* stats = new screen;
    screen* settings = new screen;
    screen* editor = new screen;
    const int& width = this->getWindowInfo().w();
    const int& height = this->getWindowInfo().h();
    SDL_Texture* textTexture = createTTFTexture(this->getRenderer(),this->getFont("impact.ttf"),"TEST TEST TEST",{0,255,0,255});

    uiTextureComponent* uiTestTexture = new uiTextureComponent(
        textTexture,
        fRect(width-width/10,height/6,600,110)
    );

    uiTTFComponent* uiTestTTF = new uiTTFComponent(
        this,
        "TEST",
        pos(100,100),
        100,
        this->getFont("impact.ttf"),
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
        this,
        onClickDebug,
        fRect(width-width/10,height/2,300,100),
        color(255,255,255,255),
        "TESTBTN",
        this->getFont("impact.ttf"),
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
    
    uiButtonRectText* uiNew = new uiButtonRectText(
        this,
        nullptr,
        fRect(width/20,height/4,200,90),
        color(255,255,255,255),
        "New",
        this->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    uiButtonRectText* uiEdit = new uiButtonRectText(
        this,
        nullptr,
        fRect(width/20,height/2.8,200,90),
        color(255,255,255,255),
        "Edit",
        this->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    float spacing = (height / 2.8) - (height / 4);

    uiButtonRectText* uiTestMap = new uiButtonRectText(
        this,
        nullptr,
        fRect(width/20, (height/2.8) + spacing, 200, 90),
        color(255,255,255,255),
        "Test",
        this->getFont("impact.ttf"),
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
        this,
        [](uiButton*){
            app->appRunning = false;
        },
        fRect(width/20, (height/2.8) + 3*spacing, 200, 90),
        color(255,255,255,255),
        "Quit",
        this->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    uiButtonRectText* uiMaxFps = new uiButtonRectText(
        this,
        [](uiButton*){
            unsigned short newFPS;
            bool fail;
            do{
            std::cout << "Enter a new maximum framerate (max : 500)\n";
            std::cin >> newFPS;

            fail = std::cin.fail();
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            } while(newFPS<1 || newFPS >= 500 || fail);
            app->setMaxFramerate(newFPS);
        },
        fRect(width/2-width/12,height/4,300,90),
        color(255,255,255,255),
        "Max Framerate",
        this->getFont("impact.ttf"),
        60,
        color(0,0,0,255)
    );

    uiButtonRectText* uiSettingsBack = new uiButtonRectText(
        this,
        [](uiButton*){
            app->getUi()["mainMenu"]->setEnabled(true);
            app->getUi()["settings"]->setEnabled(false);
        },
        fRect(width/20,height/4,200,90),
        color(255,255,255,255),
        "Back",
        this->getFont("impact.ttf"),
        80,
        color(0,0,0,255)
    );

    (*mainMenu)["New"] = uiNew;
    (*mainMenu)["Edit"] = uiEdit;
    (*mainMenu)["Test"] = uiTestMap;
    (*mainMenu)["Settings"] = uiSettings;
    (*mainMenu)["Quit"] = uiQuit;
    (*stats)["FPS"] = uiFPS;
    (*stats)["WinInfo"] = uiWinInfo;
    (*debug)["Test"] = uiTest;
    (*debug)["textureTest"] = uiTestTexture;
    (*debug)["ttfTest"] = uiTestTTF;
    (*debug)["testGlobalTexture"] = uiTestGlobalTexture;
    (*debug)["testButton"] = uiTestButton;
    (*debug)["testButtonText"] = uiTestTextButton;
    (*debug)["testButtonTexture"] = uiTestTexureButton;
    (*settings)["MaxFps"] = uiMaxFps;
    (*settings)["Back"] = uiSettingsBack;

    insertScreen("mainMenu", mainMenu);
    insertScreen("debug", debug);
    insertScreen("stats", stats);
    insertScreen("settings", settings);

    getUi()["debug"]->setEnabled(false);
    getUi()["settings"]->setEnabled(false);
    
}
