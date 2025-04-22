#include <map.h>
#include <color.h>

extern std::map<std::string,SDL_Texture*> globalTextures;

void map::render(bool renderRect, color rectColor) {
    if(rendered){
        float zoom = cam.getZoom();
        size_t tilePixelSize = (tileSize * renderTarget.w) / app->getWindowInfo().w(); // Cross-multiplication
        tilePixelSize = static_cast<size_t>(tilePixelSize * zoom);

        float _mapBeginX = mapBeginX();
        float _mapBeginY = mapBeginY();

        for (long long x = 0; x < tiles.getWidth(); ++x) {
            float current_x = _mapBeginX + x * tilePixelSize;
            for (long long y = 0; y < tiles.getHeight(); ++y) {
                float current_y = _mapBeginY + y * tilePixelSize;

                fRect srcRect;
                fRect dstRect;

                tile t = tiles.getTile(x, y);
                SDL_Texture* texture = globalTextures[t.getTextureName()];
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
        if(renderRect){
            setRenderDrawColor(app->getRenderer(), rectColor);
            SDL_RenderRect(app->getRenderer(),&renderTarget);
        }

        ply.render();
    }
}