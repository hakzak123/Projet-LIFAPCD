#pragma once
#include <SDL3/SDL.h>
#include <windowInfo.h>
#include <position.h>
#include <application.h>

class uiComponent{
    protected :
        SMM* app;
        pos Pos;
        bool rendered = true;

    public :
        uiComponent(){}

        uiComponent(SMM* _app, pos _Pos) : app(_app), Pos(_Pos){
    
        }
    
        bool isRendered(){
            return rendered;
        }
    
        void renderable(bool b){
            rendered = b;
        }
    
        pos getPos(){
            return Pos;
        }

        void setPos(const pos& _Pos){
            Pos = _Pos;
        }

        virtual void update() = 0;
        virtual void render() = 0;
    };