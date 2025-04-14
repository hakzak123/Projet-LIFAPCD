#pragma once
#include <position.h>

class mapComponent{
    private:
        pos Pos;
        bool collision;
    public :
        mapComponent(const pos& _Pos = pos(0,0), bool _collisions = true) : Pos(_Pos), collision(_collisions){}
        
        void setCollision(bool b){
            collision = b;
        }
        bool getCollision() const{
            return collision;
        }
        void setPos(const pos& _Pos){
            Pos = _Pos;
        }
        pos getPos() const{
            return Pos;
        }
    };