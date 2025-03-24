#pragma once
#include <SDL3/SDL.h>
#include <initializer_list>

class color{
    public:
    Uint8 r=0, g=0, b=0, a=0;

    color(){}

    color(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a): r(_r), g(_g), b(_b), a(_a){}
};

inline bool setRenderDrawColor(SDL_Renderer* renderer, color p){
    return SDL_SetRenderDrawColor(renderer,p.r,p.g,p.b,p.a);
}


