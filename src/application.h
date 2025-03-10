#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <windowInfo.h>

class SMM{
public:
    SDL_Window *window = nullptr;
    SDL_Renderer* renderer = nullptr;
    windowInfo winfo;
    bool appRunning = true;
    double framerate = 0;
    double maxframerate = 60;
    Uint64 framecount = 0;

    void setup(){
        
    }
};