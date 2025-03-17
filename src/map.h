#pragma once
#include <SDL3/SDL.h>
#include <application.h>
#include <tile.h>
#include <spawnpoint.h>

class mapComponent{
    
};

class map{
private :
    SMM* app;
    tileMap tiles;
    unsigned tileSize;
    spawnPoint spawn;

public :
    map(SMM* _app, const tileMap& t);
    void update(); // camera is in app
    void render();

};