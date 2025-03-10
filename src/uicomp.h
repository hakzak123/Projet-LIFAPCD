#pragma once
#include <SDL3/SDL.h>
#include <windowInfo.h>
#include <position.h>

class uiComponent{
    protected :
        SDL_Renderer* renderer;
        windowInfo* winfo;
        pos Pos;
        bool rendered = true;
    public :
    
        uiComponent(SDL_Renderer* _renderer, windowInfo* _winfo, pos _Pos) : renderer(_renderer), winfo(_winfo), Pos(_Pos){
    
        }
    
        bool isRendered(){
            return rendered;
        }
    
        void renderable(bool b){
            rendered = b;
        }
    
        virtual void update() = 0;
        virtual void render() = 0;
    };