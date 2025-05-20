#include <SDL3/SDL.h>
#include <mapcomponent.h>
#include <string>
#include <map.h>
#include <map>

extern map g_map;
extern SMM* app;

void player::render(){
    size_t tileSize = g_map.getTileSize();
    size_t tilePixelSize = (tileSize * g_map.getRenderTarget().w) / app->getWindowInfo().w();
    tilePixelSize = static_cast<size_t>(tilePixelSize * g_map.getCamera().getZoom());
    fRect dstRect(0,0,0,0);
    fRect srcRect(0,0,0,0);
    float hitboxWidthInPixels = hitbox.w * tilePixelSize/tileSize;
    float hitboxHeightInPixels = hitbox.h * tilePixelSize/tileSize;

    dstRect.x = g_map.worldXtoWindowX(Pos.x-hitbox.w/2);
    dstRect.y = g_map.worldYtoWindowY(Pos.y-hitbox.h/2);
    dstRect.w = hitboxWidthInPixels;
    dstRect.h = hitboxHeightInPixels;

    SDL_RenderTexture(app->getRenderer(), app->getGlobalTextures()[sprite], NULL, &dstRect);
}

void player::moveRight(){
    Pos.x += speed;
}

void player::moveLeft(){
    Pos.x -= speed;    

}

void player::moveUp(){
    Pos.y -= speed;
}

void player::moveDown(){
    Pos.y += speed;
}