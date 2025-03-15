#pragma once
#include <SDL3/SDL.h>
#include <application.h>
#include <tile.h>

class mapComponent{
    
};

class map{
private :
    SMM* app;
    tileMap tiles;

public :

    void update();
    void render();

};