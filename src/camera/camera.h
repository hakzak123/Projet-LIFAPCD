#pragma once
#include <SDL3/SDL.h>
#include <position.h>

class camera{
private :
    pos Pos;
    float editorSpeed = 15;
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

    float getZoom(){
        return zoom;
    }

    void setZoom(float _zoom){
        if(_zoom > 0.1 && _zoom < 10)
            zoom = _zoom;
    }

    float getEditorSpeed(){
        return editorSpeed;
    }

    void setEditorSpeed(float speed){
        editorSpeed = speed;
    }
};