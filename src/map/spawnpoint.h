#pragma once
#include <SDL3/SDL.h>
#include <position.h>
#include <map.h>

class spawnPoint : public mapComponent{
public:
    spawnPoint(const pos &_Pos) : mapComponent(_Pos, false){}

};