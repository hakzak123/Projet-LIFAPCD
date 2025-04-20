#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <chrono>
#include <windowInfo.h>
#include <macros.h>
#include <camera.h>

class screen;

class SMM{
private:
    SDL_Cursor* handCursor = nullptr;
    SDL_Cursor* arrowCursor = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::map<std::string,TTF_Font*> fontMap;
    windowInfo winfo;
    double framerate = 0;
    double maxframerate = 60;
    std::map<std::string,screen*> ui;
    
public:
    const std::string workspace = std::string(SDL_GetBasePath()) + "../";
    bool appRunning = true;

    SMM(){
    }

    ~SMM(){
        if(handCursor)
            SDL_DestroyCursor(handCursor);
        if(arrowCursor)
            SDL_DestroyCursor(arrowCursor);
    }

    SDL_Window* const getWindow(){
        return window;
    }
    SDL_Renderer* const getRenderer(){
        return renderer;
    };
    void addFont(const std::string& key,TTF_Font* font){
        fontMap[key] = font;
    }
    TTF_Font* getFont(const std::string& key){
        return fontMap[key];
    }
    const windowInfo& getWindowInfo(){
        return winfo;
    }
    double getFramerate() const{
        return framerate;
    }
    double getMaxFramerate()const{
        return maxframerate;
    }
    void setMaxFramerate(double fps){
        maxframerate = fps;
    }
    std::map<std::string,screen*>& getUi(){
        return ui;
    }
    void insertScreen(std::string key, screen* _screen){
        ui[key] = _screen;
    }

    void init(const std::string& name, int _w, int _h, SDL_WindowFlags _flags){
        // fontMap init
        SDL_EnumerateDirectory((workspace + "data/fonts").c_str(),
        [](void* data, const char *dirname, const char *fname) -> SDL_EnumerationResult{
            typedef std::map<std::string, TTF_Font *> font_map;
            TTF_Font* font;
            if(!(font = TTF_OpenFont(((std::string)dirname + fname).c_str(),256))){
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

    // handles update rate and sets framerate
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

    void argumentHandling(SMM*,int,char**);
    void cursorHandling();
    void uiSetup();
    void update();
    void render();


};