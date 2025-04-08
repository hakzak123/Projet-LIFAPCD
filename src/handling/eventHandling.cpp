#include <SDL3/SDL.h>
#include <iostream>
#include <application.h>
#include <ui.h>

extern SMM* app;

void eventHandling(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_EVENT_QUIT: {
                app->appRunning = false;
                break;
            }
        }
        for(auto& e : app->getUi()){
            e.second->eventHandler(event);
        }

    }
}