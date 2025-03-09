// plan : faire un map maker en 2D + personnage avec controles basiques et si le temps faire un jeu avec
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <macros.h>
#include <windowInfo.h>

SDL_Window *window = nullptr;
windowInfo winfo;
TTF_Font* testFont;

double framerate = 0;
double maxframerate = 60;
Uint64 framecount = 0;
std::string workspace = std::string(SDL_GetBasePath()) + "../";


void argumentHandling(int,char**);
void render(SDL_Renderer*);
void uiSetup(SDL_Renderer*);
void update();


int main(int argc, char* argv[]) {
    argumentHandling(argc,argv);
    double maxframeratecyclecount = 1000000000/maxframerate;
    bool gameRunning = true;
    SDL_Event event;
    SDL_Renderer* renderer;

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_ShowSimpleMessageBox(0,"ERROR!", "SDL_Init() failed",nullptr);
        return 0;
    }

    if(!TTF_Init()){
        SDL_ShowSimpleMessageBox(0,"ERROR!", "TTF_Init() failed",nullptr);
        return 0;
    }

    testFont = TTF_OpenFont((workspace + "data/fonts/testTTF.ttf").c_str(),256);

    if(window = SDL_CreateWindow("SDL test",0,0,SDL_WINDOW_FULLSCREEN)){
        renderer = SDL_CreateRenderer(window,NULL);
    }
    else{
        SDL_ShowSimpleMessageBox(0,"ERROR!", "Window creation failed failed",nullptr);
    }

    winfo = window;
    uiSetup(renderer);
    

    while(gameRunning){
        auto frameStart = CURRENT_TIME;

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_QUIT: {
                    gameRunning = false;
                    break;
                }
            }
        }

        update();
        render(renderer);

        std::chrono::duration<double> frameTime = CURRENT_TIME-frameStart;
        double frameTimeMS = 1000000000*frameTime.count();
        if(frameTimeMS<maxframeratecyclecount){
            SDL_DelayNS(maxframeratecyclecount-frameTimeMS);
        }

        std::chrono::duration<double> loopTime = CURRENT_TIME-frameStart;
        framerate = 1/loopTime.count();
        framecount++;
    }

    SDL_Quit();
}