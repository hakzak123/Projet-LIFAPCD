#pragma once
#include <SDL3/SDL.h>
#include <position.h>
#include <geometry.h>
#include <mapcomponent.h>

class player : public mapComponent{
private :
    std::string sprite;
    fRect hitbox; // en worldPos
    int speed;

    void updatePhys();

public :
    player(const std::string &_sprite = "player_sprite.bmp", const fRect &_hitbox = fRect(0,0,50,100), const pos &_Pos = pos(0,0), int _speed = 15, bool _collision = true):
    mapComponent(_Pos, _collision),
    sprite(_sprite),
    hitbox(_hitbox),
    speed(_speed)
    {}

    void update();
    void render();

    void moveRight();
    void moveLeft(){
        Pos.x -= speed;  
    }
    void jump();

    void setPos(pos _Pos);
    pos getPos();
    void setSpeed(int _speed);
    int getSpeed();

};