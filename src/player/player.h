#pragma once
#include <SDL3/SDL.h>
#include <position.h>
#include <geometry.h>
#include <map.h>
#include <string>

class player : public mapComponent{
private :
    std::string sprite;
    fRect hitbox;
    int speed;

    void updatePhys();

public :
    player(const std::string &_sprite,const fRect &_hitbox, const pos &_Pos, int _speed, bool _collision = true):
    mapComponent(_Pos, _collision),
    sprite(_sprite),
    hitbox(_hitbox),
    speed(_speed)
    {}

    void update();
    void render();

    void moveRight();
    void moveLeft();
    void jump();

    void setPos(pos _Pos);
    pos getPos();
    void setSpeed(int _speed);
    int getSpeed();

};