#pragma once
#include <SDL3/SDL.h>

class fRect : public SDL_FRect{
public: 
    fRect(){
    }

    fRect(float _x, float _y, float _w, float _h){
        x = _x;
        y = _y;
        w = _w;
        h = _h;
    }

};

class rect : public SDL_Rect{
    public: 
        rect(){
    
        }
    
        rect(int _x, int _y, int _w, int _h){
            x = _x;
            y = _y;
            w = _w;
            h = _h;
        }
    
    };