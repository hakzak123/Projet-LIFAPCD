#pragma once
#include <SDL3/SDL.h>
#include <position.h>
#include <map.h>

class spawnPoint : public mapComponent{
private:
    pos Pos;

public:
    spawnPoint(const pos &_Pos);

};