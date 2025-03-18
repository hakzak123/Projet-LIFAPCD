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
    int speed;
    bool collision = true;
    
    void updatePhys();

public :
    player(const std::string &_sprite,const fRect &_hitbox, const pos &_Pos, int _speed, bool _collision = true);


    void update();
    void render();

    void moveRight();
    void moveLeft();
    void jump();

    void setPos(pos _Pos);
    pos getPos();
    void setSpeed(int _speed);
    int getSpeed();
    void setCollision(bool b);
    bool getCollision();
};