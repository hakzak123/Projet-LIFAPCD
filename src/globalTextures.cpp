#include <SDL3/SDL.h>
#include <map>
#include <application.h>
#include <smartptr.h>

extern SMM* app;
std::map<std::string,SDL_Texture*> globalTextures;

SDL_EnumerationResult callback(void* data, const char *dirname, const char *fname){
    uniqueSDLPtr<SDL_Surface> surface = SDL_LoadBMP((app->workspace+"data/textures/" + fname).c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->getRenderer(),surface.pointer());
    globalTextures[fname] = texture;
    return SDL_ENUM_CONTINUE;
}

void initGlobalTextures(){
    SDL_EnumerateDirectory((app->workspace + "data/textures").c_str(),callback,nullptr);
}

void destroyGlobalTextures(){
    for(auto &e : globalTextures){
        SDL_DestroyTexture(e.second);
        std::cout << "successfully destroyed " << e.first << '\n';
    }
}