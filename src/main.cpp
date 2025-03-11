// plan : faire un map maker en 2D + personnage avec controles basiques et si le temps faire un jeu avec
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <macros.h>
#include <windowInfo.h>
#include <application.h>

void argumentHandling(SMM*,int,char**);
void eventHandling(SMM*,const SDL_Event&);
void render(SMM*);
void uiSetup(SDL_Renderer*);
void update(SMM*);

SMM* app;

int main(int argc, char* argv[]){
    app = new SMM;
    argumentHandling(app,argc,argv);
    SDL_Event event;

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_ShowSimpleMessageBox(0,"ERROR!", "SDL_Init() failed",nullptr);
        return 0;
    }

    if(!TTF_Init()){
        SDL_ShowSimpleMessageBox(0,"ERROR!", "TTF_Init() failed",nullptr);
        return 0;
    }
    
    app->setup();

    while(app->appRunning){
        auto frameStart = CURRENT_TIME;

        while(SDL_PollEvent(&event)){
            eventHandling(app,event);
        }

        app->update();
        app->render();
        app->framerateHandling(frameStart);
    }

    SDL_Quit();
    delete app;
}