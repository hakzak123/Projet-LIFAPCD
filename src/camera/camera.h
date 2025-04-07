#pragma once
#include <SDL3/SDL.h>
#include <position.h>

class camera{
private :
    pos Pos;
    float zoom = 1;
public :
    camera(){}
    camera(pos _Pos, float _zoom = 1) : Pos(_Pos), zoom(_zoom) {}

    pos getPos(){
        return Pos;
    }

    void setPos(const pos& _Pos){
        Pos = _Pos;
    }
};