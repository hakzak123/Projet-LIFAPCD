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

//        ply.setPos(spawn.getPos());
    }


    void update(){
        
        int windowWidth = app->getWindowInfo().w();
        int windowHeight = app->getWindowInfo().h();

        renderTarget.x = renderTarget.iniX()*windowWidth/app->getWindowInfo().iniW();
        renderTarget.y = renderTarget.iniY()*windowHeight/app->getWindowInfo().iniH();
        renderTarget.w = renderTarget.iniW() / app->getWindowInfo().iniW() * windowWidth;
        renderTarget.h = renderTarget.iniH() / app->getWindowInfo().iniH() * windowHeight;

        if(testMode){
            cam.setPos(pos(ply.getPos().x, -ply.getPos().y));
            if(!isInRectangle(ply.getPos(),fRect(-offsetX(), -offsetY(), tileSize*tiles.getWidth(), tileSize*tiles.getHeight()))){
                ply.setPos(spawn.getPos());
            }
        }
    }

    void render(bool, color rectColor = color(0,255,0,255));

    float mapBeginX(){
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
        tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());
    
        float halfRenderTargetWidth = renderTarget.w / 2.0;
        float halfMapWidthInPixels = (tilePixelSize * tiles.getWidth()) / 2.0;
        float cameraOffsetX = cam.getPos().x * tilePixelSize / tileSize;
        return renderTarget.x + halfRenderTargetWidth - halfMapWidthInPixels - cameraOffsetX;
        
    }

    float mapEndX(){
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
        tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());
    
        float halfRenderTargetWidth = renderTarget.w / 2.0;
        float halfMapWidthInPixels = (tilePixelSize * tiles.getWidth()) / 2.0;
        float cameraOffsetX = cam.getPos().x * tilePixelSize / tileSize;
        return renderTarget.x + halfRenderTargetWidth + halfMapWidthInPixels - cameraOffsetX;
        
    }

    float mapBeginY(){
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
        tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());
    
        float halfRenderTargetHeight = renderTarget.h / 2.0;
        float halfMapHeightInPixels = (tilePixelSize * tiles.getHeight()) / 2.0;
        float cameraOffsetY = cam.getPos().y * tilePixelSize / tileSize;
        return renderTarget.y + halfRenderTargetHeight - halfMapHeightInPixels + cameraOffsetY;        
    }

    float mapEndY(){
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
        tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());
    
        float halfRenderTargetHeight = renderTarget.h / 2.0;
        float halfMapHeightInPixels = (tilePixelSize * tiles.getHeight()) / 2.0;
        float cameraOffsetY = cam.getPos().y * tilePixelSize / tileSize;
        return renderTarget.y + halfRenderTargetHeight + halfMapHeightInPixels + cameraOffsetY;        
    }

    float windowXtoWorldX(float windowX) {
        float _mapBeginX = mapBeginX();
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
        tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

        float scale = static_cast<float>(tileSize) / tilePixelSize;
    
        float worldX = (windowX - _mapBeginX) * scale;
    
        float distanceToOrigin = worldX - offsetX();
    
        return distanceToOrigin;
    }

    float windowYtoWorldY(float windowY) {
        float _mapBeginY = mapBeginY();
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
        tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

        float scale = static_cast<float>(tileSize) / tilePixelSize;
    
        float worldY = (windowY - _mapBeginY) * scale;
    
        float distanceToOrigin = worldY - offsetY();
    
        return distanceToOrigin;
    }

    float worldXtoWindowX(float worldX) {
        float _mapBeginX = mapBeginX();
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
        tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());
    
        float scale = static_cast<float>(tilePixelSize) / tileSize;

        return _mapBeginX + (worldX + offsetX()) * scale;
    }

    float worldYtoWindowY(float worldY) {
        float _mapBeginY = mapBeginY();
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
        tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());
    
        float scale = static_cast<float>(tilePixelSize) / tileSize;

        return _mapBeginY + (worldY + offsetY()) * scale;
    }

    tile* getTileFromWinPos(float windowX, float windowY){
        float offsetWorldX = windowXtoWorldX(windowX) + offsetX();
        float offsetWorldY = windowYtoWorldY(windowY) + offsetY();

        if(offsetWorldX < 0 || offsetWorldX > offsetX() * 2 || offsetWorldY < 0 || offsetWorldY > offsetY() * 2)
            return NULL;
    
        int xIndex = offsetWorldX/tileSize;
        int yIndex = offsetWorldY/tileSize;

        return &tiles.getTile(xIndex, yIndex);
    }

    tile* getTileFromWorldPos(float worldX, float worldY){
        float offsetWorldX = worldX + offsetX();
        float offsetWorldY = worldY + offsetY();

        if(offsetWorldX < 0 || offsetWorldX > offsetX() * 2 || offsetWorldY < 0 || offsetWorldY > offsetY() * 2)
            return NULL;
    
        int xIndex = offsetWorldX/tileSize;
        int yIndex = offsetWorldY/tileSize;

        return &tiles.getTile(xIndex, yIndex);
    }

    void eventHandler(const SDL_Event& event){
        switch(event.type){
            case SDL_EVENT_MOUSE_WHEEL: {
                if (event.wheel.y > 0) {
                    cam.setZoom(cam.getZoom() * 1.1f);
                } else if (event.wheel.y < 0) {
                    cam.setZoom(cam.getZoom() * 0.9f);
                }
                break;
            }
        }
    }

    void directionHandler(){
        float _mapBeginX = mapBeginX();
        float _mapBeginY = mapBeginY();
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
        tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());
        float tileRatio = static_cast<float>(tilePixelSize)/tileSize;
        float halfRenderTargetHeight = renderTarget.h / 2.0;
        float halfRenderTargetWidth = renderTarget.w / 2.0;
        float MapHeightInPixels = tilePixelSize * tiles.getHeight();
        float MapWidthInPixels = tilePixelSize * tiles.getWidth();
        float halfMapHeightInPixels = MapHeightInPixels/ 2.0;
        float halfMapWidthInPixels = MapWidthInPixels/ 2.0;

        const bool* keystate = SDL_GetKeyboardState(NULL);

        if(keystate[SDL_SCANCODE_W]){
            if(testMode){
                ply.moveUp();
            }
            else{
                pos newPos = cam.getPos();
                newPos.y += cam.getEditorSpeed();
                cam.setPos(newPos);
            }

        }
        if(keystate[SDL_SCANCODE_A]){
            if(testMode){
                ply.moveLeft();
            }
            else{
                pos newPos = cam.getPos();
                newPos.x -= cam.getEditorSpeed();
                cam.setPos(newPos);
            }
        }
        if(keystate[SDL_SCANCODE_S]){
            if(testMode){
                ply.moveDown();
            }
            else
            {
                pos newPos = cam.getPos();
                newPos.y -= cam.getEditorSpeed();
                cam.setPos(newPos);
            }

        }
        if(keystate[SDL_SCANCODE_D]){
            if(testMode){
                ply.moveRight();
            }
            else{
                pos newPos = cam.getPos();
                newPos.x += cam.getEditorSpeed();
                cam.setPos(newPos);
            }
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