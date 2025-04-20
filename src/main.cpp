// plan : faire un map maker en 2D + personnage avec controles basiques et si le temps faire un jeu avec
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <macros.h>
#include <application.h>

void eventHandling();
void initGlobalTextures();
void destroyGlobalTextures();
void mapInit(SMM*);

SMM* app;

int main(int argc, char* argv[]){
    app = new SMM;
    app->argumentHandling(app,argc,argv);

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_ShowSimpleMessageBox(0,"ERROR!", "SDL_Init() failed",nullptr);
        return 0;
    }

    if(!TTF_Init()){
        SDL_ShowSimpleMessageBox(0,"ERROR!", "TTF_Init() failed",nullptr);
        return 0;
    }

    app->init("SMM", 0, 0, SDL_WINDOW_FULLSCREEN);
    initGlobalTextures();
    mapInit(app);
    app->uiSetup();



    while(app->appRunning){
        auto frameStart = CURRENT_TIME;

        eventHandling();
        
        app->update();
        app->render();
        app->framerateHandling(frameStart);
    }

    destroyGlobalTextures();
    delete app;
    SDL_Quit();
}