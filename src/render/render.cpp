#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <color.h>
#include <ui.h>
#include <geometry.h>
#include <application.h>

void SMM::render(){
    setRenderDrawColor(getRenderer(),color(0,0,0,255));
    SDL_RenderClear(getRenderer());
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    setRenderDrawColor(getRenderer(),color(50,50,50,70));
    SDL_RenderFillRect(getRenderer(),NULL);

    for(auto& e : getUi()){
        e.second->render(this);
    }

    SDL_RenderPresent(getRenderer());
}