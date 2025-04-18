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
    std::string filePath;

public :
    map(SMM* _app, const tileMap& t, spawnPoint _spawn, unsigned _tileSize = 50, std::string _filePath = std::string(), player _ply = player()) :
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
    tileMap& getTileMap(){
        return tiles;
    }
    player& getPlayer(){
        return ply;
    }
    spawnPoint getSpawnPoint(){
        return spawn;
    }
    void setSpawnPoint(const spawnPoint& _spawn){
        spawn = _spawn;
    }
    unsigned getTileSize(){
        return tileSize;
    }
    void setTileSize(unsigned _tileSize){
        tileSize = _tileSize;
    }
    std::string getFilePath(){
        return filePath;
    }
    void setFilePath(const std::string& path){
        filePath = path;
    }

};