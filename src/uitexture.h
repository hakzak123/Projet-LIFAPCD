#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <uicomp.h>
#include <geometry.h>




class uiTextureComponent : public uiComponent{ // corrects a texture's rectangle position so that it doesn't overflow to the right. Doesn't take in src rectangle for simplicity.
protected :
    SDL_Texture* texture;
    fRect dstRect;

public :
    uiTextureComponent(SDL_Renderer* _renderer, SDL_Texture* _texture, windowInfo* _winfo,fRect _dstRect):
    uiComponent(_renderer,_winfo,pos(_dstRect.x,_dstRect.y)),
    texture(_texture),
    dstRect(_dstRect)
    {
    }


    void update() override{
        Pos.x = Pos.iniX()*winfo->w()/winfo->iniW();
        Pos.y = Pos.iniY()*winfo->h()/winfo->iniH();
        dstRect.x = dstRect.iniX()*winfo->w()/winfo->iniW();
        dstRect.y = dstRect.iniY()*winfo->h()/winfo->iniH();
    }

    void render(){
        if(rendered){
            fRect tmpRect = dstRect;
            size_t rectWidth = dstRect.w;
            float gap = winfo->w()-dstRect.x;
            float overflow = rectWidth-gap;
            bool positiveOverflow = overflow > 0 ? true : false;
            
            if(positiveOverflow){
                tmpRect.x -= overflow;
            }
            SDL_RenderTexture(renderer,texture,NULL,&tmpRect);
        }
    }

};

class uiTTFComponent : public uiTextureComponent{
    protected:
        unsigned height;
        std::string text;
        TTF_Font* font;
    
    public:
    
        void render() override;
    };