#include <SDL3/SDL.h>
#include <map>
#include <application.h>
#include <smartptr.h>

SDL_EnumerationResult callback(void* data, const char *dirname, const char *fname){
    SMM* _app = (SMM*)data;
    std::string fnameStr = fname;

    if(fnameStr.substr(fnameStr.size()-4) != ".bmp"){
        return SDL_ENUM_CONTINUE;
    }

    uniqueSDLPtr<SDL_Surface> surface = SDL_LoadBMP((_app->workspace+"data/textures/" + fname).c_str());

    if(!surface.pointer()){
        std::cout << "failure to load " << fnameStr << ". SDL error : " << SDL_GetError() <<'\n';
        return SDL_ENUM_CONTINUE;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(_app->getRenderer(),surface.pointer());
    _app->getGlobalTextures()[fname] = texture;
    return SDL_ENUM_CONTINUE;
}

void SMM::initGlobalTextures(){
    SDL_EnumerateDirectory((workspace + "data/textures").c_str(), callback, this);
    if(globalTextures.find("placeholder.bmp") == globalTextures.end()){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR!", "placeholder.bmp was not found", getWindow());
        std::exit(1);
    }
}

void SMM::destroyGlobalTextures(){
    for(auto &e : globalTextures){
        SDL_DestroyTexture(e.second);
        std::cout << "successfully destroyed " << e.first << '\n';
    }
}