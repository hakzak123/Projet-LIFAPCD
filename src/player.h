#pragma once
#include <SDL3/SDL.h>
#include <position.h>
#include <geometry.h>
#include <string>

class player{
private :
    std::string sprite;
    fRect hitbox;
    pos Pos;

public :
    player(const std::string &_sprite,const fRect &_hitbox, const pos &_Pos);

    pos getPos();
    void setPos();
};