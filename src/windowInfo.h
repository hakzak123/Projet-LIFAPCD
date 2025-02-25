#pragma once
#include <SDL3/SDL.h>
#include <iostream>

class windowInfo{ //initialisé avec une fenêtre, contient les mensurations de la fenêtre
public:
    int w;
    int h;
    SDL_Window* window;

    windowInfo(){
        
    }

    windowInfo(SDL_Window* _window) : window(_window){
        SDL_GetWindowSizeInPixels(window,&w,&h);
    }

    void operator=(SDL_Window* _window){
        window = _window;
        SDL_GetWindowSizeInPixels(window,&w,&h);
    }

    void update(){
        SDL_GetWindowSizeInPixels(window,&w,&h);
    }
};