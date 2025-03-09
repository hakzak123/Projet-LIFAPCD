#pragma once
#include <SDL3/SDL.h>

class fRect : public SDL_FRect{
protected :
    float x0 = 0, y0 = 0;
public: 
    fRect(){
    }

    fRect(float _x, float _y, float _w, float _h) : x0(_x), y0(_y){
        x = _x;
        y = _y;
        w = _w;
        h = _h;
    }


    float iniX(){
        return x0;
    }

    float iniY(){
        return y0;
    }
};

class rect : public SDL_Rect{
    protected :
    int x0 = 0 ,y0 = 0 ;
public: 
    rect(){
    }

    rect(int _x, int _y, int _w, int _h): x0(_x), y0(_y){
        x = _x;
        y = _y;
        w = _w;
        h = _h;

    }


    int iniX(){
        return x0;
    }

    int iniY(){
        return y0;
    }
};