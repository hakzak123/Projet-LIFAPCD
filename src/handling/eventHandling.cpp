#include <SDL3/SDL.h>
#include <iostream>
#include <application.h>
#include <ui.h>
#include <map.h>

extern Uint32 loadMapEvent;
extern map g_map;

void LeaveWarning(uiButton*);

void SMM::eventHandling(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_EVENT_QUIT: {
                if(!getUi()["editor"]->isEnabled())
                    appRunning = false;
                break;
            }
        }
        if(event.type == loadMapEvent){
            if(event.user.code == -1){
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","The map could not be loaded.",getWindow());
            }
        }
        for(auto& e : getUi()){
            e.second->eventHandler(event);
            if(e.first == "editor" && getUi()["editor"]->isEnabled() && event.type == SDL_EVENT_QUIT){
                LeaveWarning(nullptr);
                appRunning = false;
            }
        }
        g_map.eventHandler(event);
    }
    g_map.directionHandler();
}