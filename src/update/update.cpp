#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <ui.h>
#include <windowInfo.h>
#include <application.h>
#include <map.h>

extern map g_map;

void SMM::update(){
    cursorHandling();
    winfo.update();
    for(auto& e : getUi()){
        e.second->update(this);
    }

    g_map.update();
}