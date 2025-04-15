#include <SDL3/SDL.h>
#include <iostream>
#include <application.h>
#include <ui.h>

extern SMM* app;
extern Uint32 loadMapEvent;

void eventHandling(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_EVENT_QUIT: {
                app->appRunning = false;
                break;
            }
        }
        if(event.type == loadMapEvent){
            if(event.user.code == -1){
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","The map could not be loaded.",app->getWindow());
            }
        }
        for(auto& e : app->getUi()){
            e.second->eventHandler(event);
        }

    }
}