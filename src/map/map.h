#pragma once
#include <SDL3/SDL.h>
#include <application.h>
#include <tile.h>
#include <spawnpoint.h>
#include <player.h>


class map{
private :
    SMM* app;
    tileMap tiles;
    unsigned tileSize;
    spawnPoint spawn;
    player ply;

public :
    map(SMM* _app, const tileMap& t, spawnPoint _spawn, unsigned _tileSize = 50, player _ply = player()) :
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
    spawnPoint& getSpawnPoint();

};