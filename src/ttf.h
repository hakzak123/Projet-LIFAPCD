#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <color.h>
#include <smartptr.h>

inline SDL_Texture* createTTFTexture(SDL_Renderer* renderer,TTF_Font* font,std::string text,color Color){
    uniqueSDLPtr<SDL_Surface> tmpSurface = TTF_RenderText_Solid(font,text.c_str(),0,{Color.r,Color.g,Color.b,Color.a});
    return SDL_CreateTextureFromSurface(renderer,tmpSurface.pointer());
}