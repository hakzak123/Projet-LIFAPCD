#pragma once
#include <SDL3/SDL.h>
#include <application.h>
#include <tile.h>
#include <spawnpoint.h>
#include <player.h>

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

class map{
private :
    SMM* app;
    tileMap tiles;
    unsigned tileSize;
    spawnPoint spawn;
    player ply;

public :
    map(SMM* _app, const tileMap& t, spawnPoint _spawn, unsigned _tileSize, player _ply);
    void update(); // camera is in app
    void render();
    tileMap& getTileMap();
    player& getPlayer();
    

};