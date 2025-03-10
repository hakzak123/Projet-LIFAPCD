#include <SDL3/SDL.h>
#include <iostream>
#include <application.h>

void eventHandling(SMM* app, const SDL_Event& event){
    switch(event.type){
        case SDL_EVENT_QUIT: {
            app->appRunning = false;
            break;
        }
    }
}