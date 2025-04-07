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
    map(SMM* _app, const tileMap& t, spawnPoint _spawn, unsigned _tileSize = 50, player _ply) :
    app(_app),
    tiles(t),
    spawn(_spawn),
    ply(_ply)
    {}
    
    void update(){
        // camera& cam = app->getCamera();
        camera cam; // à remplacer
        
    }
    void render(){
        for(const auto& tile : tiles.getTiles()){
            
        }
    }
    tileMap& getTileMap();
    player& getPlayer();
    

};