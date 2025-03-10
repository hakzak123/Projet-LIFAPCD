#include <SDL3/SDL.h>
#include <iostream>

extern bool appRunning;

void eventHandling(const SDL_Event& event){
    switch(event.type){
        case SDL_EVENT_QUIT: {
            appRunning = false;
            break;
        }
    }
}