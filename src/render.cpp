#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <color.h>
#include <ui.h>
#include <geometry.h>

extern ui Ui;


void render(SDL_Renderer* renderer){
    setRenderDrawColor(renderer,color(0,0,0,255));
    SDL_RenderClear(renderer);
    
    Ui.render();

    SDL_RenderPresent(renderer);

}