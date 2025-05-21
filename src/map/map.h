#pragma once
#include <SDL3/SDL.h>
#include <application.h>
#include <tile.h>
#include <spawnpoint.h>
#include <player.h>
#include <color.h>


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
    bool rendered = false;
    bool testMode = false;

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
    
    void init();
    void update();
    void render(bool, color rectColor = color(0,255,0,255));
    float mapBeginX();
    float mapEndX();
    float mapBeginY();
    float mapEndY();
    float windowXtoWorldX(float windowX);
    float windowYtoWorldY(float windowY);
    float worldXtoWindowX(float worldX);
    float worldYtoWindowY(float worldY);
    tile* getTileFromWinPos(float windowX, float windowY);
    tile* getTileFromWorldPos(float worldX, float worldY);
    void eventHandler(const SDL_Event& event);
    void directionHandler();

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

    fRect getRenderTarget(){
        return renderTarget;
    }

    void setRenderTarget(const fRect& target){
        renderTarget = target;
    }

    camera& getCamera(){
        return cam;
    }

    void setRendered(bool b){
        rendered = b;
    }

    bool isRendered(){
        return rendered;
    }

    bool isInTestMode(){
        return testMode;
    }

    void setTestMode(bool b){
        testMode = b;
    }

};