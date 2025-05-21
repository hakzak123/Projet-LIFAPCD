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
    std::map<std::string,SDL_Texture*> globalTextures;
    
public:
    const std::string workspace = std::string(SDL_GetBasePath()) + "../";
    bool appRunning = true;

    SMM(int, char**, const std::string&, int, int, SDL_WindowFlags);

    ~SMM(){
        if(handCursor)
            SDL_DestroyCursor(handCursor);
        if(arrowCursor)
            SDL_DestroyCursor(arrowCursor);

        destroyGlobalTextures();
        SDL_Quit();

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
    std::map<std::string,SDL_Texture*>& getGlobalTextures(){
        return globalTextures;
    }

    void windowInit(const std::string& name, int _w, int _h, SDL_WindowFlags _flags);
    void framerateHandling(std::chrono::_V2::system_clock::time_point frameStart);
    void argumentHandling(int,char**);
    void mapInit();
    void initGlobalTextures();
    void destroyGlobalTextures();
    void runLoop();
    void eventHandling();
    void cursorHandling();
    void uiSetup();
    void update();
    void render();
    int saveMapToFile(const char*);


};