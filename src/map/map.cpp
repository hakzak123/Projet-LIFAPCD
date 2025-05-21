#include <map.h>
#include <color.h>

void map::init(){
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


void map::update(){
    
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

void map::render(bool renderRect, color rectColor) {
    if(rendered){
        float zoom = cam.getZoom();
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w(); // Cross-multiplication
        tilePixelSize = static_cast<size_t>(tilePixelSize * zoom);

        float _mapBeginX = mapBeginX();
        float _mapBeginY = mapBeginY();

        long long tileIndexBeginX = (renderTarget.x - _mapBeginX) / tilePixelSize;
        long long tileIndexBeginY = (renderTarget.y - _mapBeginY) / tilePixelSize;

        long long tileIndexEndX = static_cast<long long>(tiles.getWidth()) - (mapEndX() - std::abs((renderTarget.x + renderTarget.w))) / static_cast<long long>(tilePixelSize);
        long long tileIndexEndY = static_cast<long long>(tiles.getHeight()) - (mapEndY() - std::abs((renderTarget.y + renderTarget.h))) / static_cast<long long>(tilePixelSize);

        if(
            tileIndexBeginX >= tiles.getWidth()
            || tileIndexEndX < 0
            || tileIndexBeginY >= tiles.getHeight()
            || tileIndexEndY < 0
        ){
            goto skip;
        }

        tileIndexBeginX = std::max(0LL, tileIndexBeginX);
        tileIndexBeginY = std::max(0LL, tileIndexBeginY);
        tileIndexEndX = std::min(static_cast<long long>(tiles.getWidth()) - 1, tileIndexEndX);
        tileIndexEndY = std::min(static_cast<long long>(tiles.getHeight()) - 1, tileIndexEndY);

        for (long long x = tileIndexBeginX; x <= tileIndexEndX; ++x) {
            float current_x = _mapBeginX + x * tilePixelSize;
            for (long long y = tileIndexBeginY; y <= tileIndexEndY; ++y) {
                float current_y = _mapBeginY + y * tilePixelSize;

                fRect srcRect;
                fRect dstRect;

                tile t = tiles.getTile(x, y);
                SDL_Texture* texture = app->getGlobalTextures()[t.getTextureName()];
                float textureWidth, textureHeight;
                SDL_GetTextureSize(texture, &textureWidth, &textureHeight);

                // Initialize srcRect to the full texture
                srcRect.x = 0;
                srcRect.y = 0;
                srcRect.w = textureWidth;
                srcRect.h = textureHeight;

                // Calculate dstRect
                dstRect.x = current_x;
                dstRect.y = current_y;
                dstRect.w = tilePixelSize;
                dstRect.h = tilePixelSize;

                // Skip tiles that are completely outside the renderTarget
                if (dstRect.x + dstRect.w <= renderTarget.x || dstRect.y + dstRect.h <= renderTarget.y ||
                    dstRect.x >= renderTarget.x + renderTarget.w || dstRect.y >= renderTarget.y + renderTarget.h) {
                    continue;
                }

                // Clip the source and destination rectangles if the tile overflows the renderTarget
                if (dstRect.x < renderTarget.x) {
                    float overflow = renderTarget.x - dstRect.x;
                    float clipRatio = overflow / dstRect.w;
                    srcRect.x += clipRatio * srcRect.w;
                    srcRect.w -= clipRatio * srcRect.w;
                    dstRect.w -= overflow;
                    dstRect.x = renderTarget.x;
                }

                if (dstRect.y < renderTarget.y) {
                    float overflow = renderTarget.y - dstRect.y;
                    float clipRatio = overflow / dstRect.h;
                    srcRect.y += clipRatio * srcRect.h;
                    srcRect.h -= clipRatio * srcRect.h;
                    dstRect.h -= overflow;
                    dstRect.y = renderTarget.y;
                }

                if (dstRect.x + dstRect.w > renderTarget.x + renderTarget.w) {
                    float overflow = (dstRect.x + dstRect.w) - (renderTarget.x + renderTarget.w);
                    float clipRatio = overflow / dstRect.w;
                    srcRect.w -= clipRatio * srcRect.w;
                    dstRect.w -= overflow;
                }

                if (dstRect.y + dstRect.h > renderTarget.y + renderTarget.h) {
                    float overflow = (dstRect.y + dstRect.h) - (renderTarget.y + renderTarget.h);
                    float clipRatio = overflow / dstRect.h;
                    srcRect.h -= clipRatio * srcRect.h;
                    dstRect.h -= overflow;
                }

                // Render the tile
                SDL_RenderTexture(app->getRenderer(), texture, &srcRect, &dstRect);
            }
        }
        skip:
        if(renderRect){
            setRenderDrawColor(app->getRenderer(), rectColor);
            SDL_RenderRect(app->getRenderer(),&renderTarget);
        }

        if(testMode)
            ply.render();
    }
}

float map::mapBeginX(){
    size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
    tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

    float halfRenderTargetWidth = renderTarget.w / 2.0;
    float halfMapWidthInPixels = (tilePixelSize * tiles.getWidth()) / 2.0;
    float cameraOffsetX = cam.getPos().x * tilePixelSize / tileSize;
    return renderTarget.x + halfRenderTargetWidth - halfMapWidthInPixels - cameraOffsetX;
    
}

float map::mapEndX(){
    size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
    tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

    float halfRenderTargetWidth = renderTarget.w / 2.0;
    float halfMapWidthInPixels = (tilePixelSize * tiles.getWidth()) / 2.0;
    float cameraOffsetX = cam.getPos().x * tilePixelSize / tileSize;
    return renderTarget.x + halfRenderTargetWidth + halfMapWidthInPixels - cameraOffsetX;
    
}

float map::mapBeginY(){
    size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
    tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

    float halfRenderTargetHeight = renderTarget.h / 2.0;
    float halfMapHeightInPixels = (tilePixelSize * tiles.getHeight()) / 2.0;
    float cameraOffsetY = cam.getPos().y * tilePixelSize / tileSize;
    return renderTarget.y + halfRenderTargetHeight - halfMapHeightInPixels + cameraOffsetY;        
}

float map::mapEndY(){
    size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
    tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

    float halfRenderTargetHeight = renderTarget.h / 2.0;
    float halfMapHeightInPixels = (tilePixelSize * tiles.getHeight()) / 2.0;
    float cameraOffsetY = cam.getPos().y * tilePixelSize / tileSize;
    return renderTarget.y + halfRenderTargetHeight + halfMapHeightInPixels + cameraOffsetY;        
}

float map::windowXtoWorldX(float windowX) {
    float _mapBeginX = mapBeginX();
    size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
    tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

    float scale = static_cast<float>(tileSize) / tilePixelSize;

    float worldX = (windowX - _mapBeginX) * scale;

    float distanceToOrigin = worldX - offsetX();

    return distanceToOrigin;
}

float map::windowYtoWorldY(float windowY) {
    float _mapBeginY = mapBeginY();
    size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
    tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

    float scale = static_cast<float>(tileSize) / tilePixelSize;

    float worldY = (windowY - _mapBeginY) * scale;

    float distanceToOrigin = worldY - offsetY();

    return distanceToOrigin;
}

float map::worldXtoWindowX(float worldX) {
    float _mapBeginX = mapBeginX();
    size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
    tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

    float scale = static_cast<float>(tilePixelSize) / tileSize;

    return _mapBeginX + (worldX + offsetX()) * scale;
}

float map::worldYtoWindowY(float worldY) {
    float _mapBeginY = mapBeginY();
    size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w();
    tilePixelSize = static_cast<size_t>(tilePixelSize * cam.getZoom());

    float scale = static_cast<float>(tilePixelSize) / tileSize;

    return _mapBeginY + (worldY + offsetY()) * scale;
}

tile* map::getTileFromWinPos(float windowX, float windowY){
    float offsetWorldX = windowXtoWorldX(windowX) + offsetX();
    float offsetWorldY = windowYtoWorldY(windowY) + offsetY();

    if(offsetWorldX < 0 || offsetWorldX > offsetX() * 2 || offsetWorldY < 0 || offsetWorldY > offsetY() * 2)
        return NULL;

    int xIndex = offsetWorldX/tileSize;
    int yIndex = offsetWorldY/tileSize;

    return &tiles.getTile(xIndex, yIndex);
}

tile* map::getTileFromWorldPos(float worldX, float worldY){
    float offsetWorldX = worldX + offsetX();
    float offsetWorldY = worldY + offsetY();

    if(offsetWorldX < 0 || offsetWorldX > offsetX() * 2 || offsetWorldY < 0 || offsetWorldY > offsetY() * 2)
        return NULL;

    int xIndex = offsetWorldX/tileSize;
    int yIndex = offsetWorldY/tileSize;

    return &tiles.getTile(xIndex, yIndex);
}

void map::eventHandler(const SDL_Event& event){
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

void map::directionHandler(){
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