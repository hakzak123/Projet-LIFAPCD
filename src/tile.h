#pragma once
#include <SDL3/SDL.h>
#include <map.h>

class tile : public mapComponent{
private :
    std::string textureName;
    bool collisions = true;

public :

};

class tileMap{
private :
    tile* tiles = nullptr;
    unsigned tileLenght;

public :

};