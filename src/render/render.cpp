#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <color.h>
#include <ui.h>
#include <geometry.h>
#include <application.h>

extern ui Ui;


void SMM::render(){
    setRenderDrawColor(getRenderer(),color(0,0,0,255));
    SDL_RenderClear(getRenderer());
    
    Ui.render();

    SDL_RenderPresent(getRenderer());
}