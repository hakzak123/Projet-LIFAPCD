#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <ui.h>
#include <windowInfo.h>
#include <application.h>

void SMM::update(){
    cursorHandling();
    winfo.update();
    for(auto& e : UiMap){
        e.second->update(this);
    }
}