#include <SDL3/SDL.h>
#include <iostream>
#include <application.h>
#include <ui.h>

extern ui Ui;
extern SMM* app;

void eventHandling(SDL_Event& event){
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_EVENT_QUIT: {
                app->appRunning = false;
                break;
            }
        }
        Ui.eventHandler(event);
    }
}