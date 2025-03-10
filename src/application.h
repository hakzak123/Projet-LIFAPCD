#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <chrono>
#include <windowInfo.h>
#include <macros.h>

class SMM;

void renderApp(SMM*);
void uiSetup(SMM*);
void updateApp(SMM*);


class SMM{
public:
    SDL_Window *window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::map<std::string,TTF_Font*> fontMap;

    windowInfo winfo;
    std::string workspace = std::string(SDL_GetBasePath()) + "../";

    double framerate = 0;
    double maxframerate = 60;
    Uint64 framecount = 0;
    bool appRunning = true;

    void setup(){
        // Fonts init
        TTF_Font* testFont;
        if(!(testFont = TTF_OpenFont((workspace + "data/fonts/testTTF.ttf").c_str(),256))){
            SDL_ShowSimpleMessageBox(0,"ERROR!", "Font initialization failed",nullptr);
            SDL_Quit();
            std::exit(1);
        }
        fontMap.insert({"testFont",testFont});

        // Window init
        if(window = SDL_CreateWindow("SDL test",0,0,SDL_WINDOW_FULLSCREEN)){
            renderer = SDL_CreateRenderer(window,NULL);
        }
        else{
            SDL_ShowSimpleMessageBox(0,"ERROR!", "Window creation failed failed",nullptr);
            SDL_Quit();
            std::exit(1);
        }
        winfo = window;
        
        // ui Setup
        uiSetup(this);
    }

    // handles framerate and returns current framerate
    void framerateHandling(std::chrono::_V2::system_clock::time_point frameStart){
        double maxFramerateCycleCount = 1000000000/maxframerate;
        std::chrono::duration<double> frameTime = CURRENT_TIME-frameStart;
        double frameTimeMS = 1000000000*frameTime.count();
        if(frameTimeMS<maxFramerateCycleCount){
            SDL_DelayNS(maxFramerateCycleCount-frameTimeMS);
        }

        std::chrono::duration<double> loopTime = CURRENT_TIME-frameStart;
        framerate = 1/loopTime.count();
    }

    void update(){
        updateApp(this);
    }

    void render(){
        renderApp(this);
    }

    SDL_Renderer* getRenderer(){
        return renderer;
    }
};