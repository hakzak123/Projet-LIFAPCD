#pragma once
#include <SDL3/SDL.h>
#include <iostream>

class windowInfo{ //initialisé avec une fenêtre, contient les mensurations de la fenêtre
private:
    int w0 = 0;
    int h0 = 0;
    int _w = 0;
    int _h = 0;
    SDL_Window* window;

public:
    windowInfo(){
    }

    windowInfo(SDL_Window* _window) : window(_window){
        SDL_GetWindowSizeInPixels(window,&_w,&_h);
        w0 = _w;
        h0 = _h;
    }

    void operator=(SDL_Window* _window){
        window = _window;
        SDL_GetWindowSizeInPixels(window,&_w,&_h);
        w0 = _w;
        h0 = _h;
    }

    void update(){
        SDL_GetWindowSizeInPixels(window,&_w,&_h);
    }

    int w(){
        return _w;
    }

    int h(){
        return _h;
    }

    int iniW(){
        return w0;
    }

    int iniH(){
        return h0;
    }
};