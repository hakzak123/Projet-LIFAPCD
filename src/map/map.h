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
    camera cam;
    fRect renderTarget;

    long long offsetX(){
        return (tiles.getWidth()*tileSize)/2;
    }

    long long offsetY(){
        return (tiles.getHeight()*tileSize)/2;
    }

public :
    map(SMM* _app, const tileMap& t, spawnPoint _spawn, unsigned _tileSize = 100, std::string _filePath = std::string(), player _ply = player()) :
    app(_app),
    tileSize(_tileSize),
    tiles(t),
    spawn(_spawn),
    ply(_ply)
    {}
    
    void init(){
        unsigned width = tiles.getWidth(), height = tiles.getHeight();
        for(long long x = 0; x < width; ++x){
            for(long long y = 0; y < height; ++y){
                tiles.getTile(x,y).setPos(pos(
                    tileSize/2 + x* tileSize - offsetX(),
                    tileSize/2 + y* tileSize - offsetY()
                ));
            }
        }
    }

    void update(){

    }

    void render();

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

    void setRenderTarget(const fRect& target){
        renderTarget = target;
    }

};