#include <application.h>

SMM::SMM(int argc, char* argv[], const std::string& windowName, int width, int height, SDL_WindowFlags flags){
    argumentHandling(argc, argv);

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_ShowSimpleMessageBox(0,"ERROR!", "SDL_Init() failed",nullptr);
        std::exit(1);
    }

    if(!TTF_Init()){
        SDL_ShowSimpleMessageBox(0,"ERROR!", "TTF_Init() failed",nullptr);
        std::exit(1);
    }

    windowInit(windowName, width, height, flags);
    initGlobalTextures();
    mapInit();
    uiSetup();

}

void SMM::windowInit(const std::string& name, int _w, int _h, SDL_WindowFlags _flags){
    // fontMap init
    SDL_EnumerateDirectory((workspace + "data/fonts").c_str(),
    [](void* data, const char *dirname, const char *fname) -> SDL_EnumerationResult{
        typedef std::map<std::string, TTF_Font *> font_map;
        TTF_Font* font;
        if(!(font = TTF_OpenFont(((std::string)dirname + fname).c_str(),64))){
            SDL_ShowSimpleMessageBox(0,"ERROR!", "Font initialization failed",nullptr);
            SDL_Quit();
            std::exit(1);
        }
        static_cast<font_map*>(data)->insert({fname, font});
        return SDL_ENUM_CONTINUE;
    },
    &fontMap);

    // Window init
    if(window = SDL_CreateWindow(name.c_str(),_w,_h,_flags)){
        renderer = SDL_CreateRenderer(window,NULL);
    }
    else{
        SDL_ShowSimpleMessageBox(0,"ERROR!", "Window creation failed failed",nullptr);
        SDL_Quit();
        std::exit(1);
    }
    winfo = window;
}

void SMM::runLoop(){
    while(appRunning){
        auto frameStart = CURRENT_TIME;

        eventHandling();     
        update();
        render();
        framerateHandling(frameStart);
    }
}

void SMM::framerateHandling(std::chrono::_V2::system_clock::time_point frameStart){
    double maxFramerateCycleCount = 1000000000/maxframerate;
    std::chrono::duration<double> frameTime = CURRENT_TIME-frameStart;
    double frameTimeMS = 1000000000*frameTime.count();
    if(frameTimeMS<maxFramerateCycleCount){
        SDL_DelayNS(maxFramerateCycleCount-frameTimeMS);
    }

    std::chrono::duration<double> loopTime = CURRENT_TIME-frameStart;
    framerate = 1/loopTime.count();
}