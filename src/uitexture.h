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
    uiTextureComponent(SMM* _app, SDL_Texture* _texture,fRect _dstRect):
    uiComponent(_app,pos(_dstRect.x,_dstRect.y)),
    texture(_texture),
    dstRect(_dstRect)
    {
    }


    void update() override{
        windowInfo winfo = app->winfo;
        SDL_Renderer* renderer = app->renderer;
        Pos.x = Pos.iniX()*winfo.w()/winfo.iniW();
        Pos.y = Pos.iniY()*winfo.h()/winfo.iniH();
        dstRect.x = dstRect.iniX()*winfo.w()/winfo.iniW();
        dstRect.y = dstRect.iniY()*winfo.h()/winfo.iniH();
    }

    void render(){
        if(rendered){
            windowInfo winfo = app->winfo;
            SDL_Renderer* renderer = app->renderer;
            fRect tmpRect = dstRect;
            size_t rectWidth = dstRect.w;
            float gap = winfo.w()-dstRect.x;
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
        std::string text;
        unsigned height;
        TTF_Font* font;
    
    public:
        uiTTFComponent(SMM* _app, std::string _text ,pos _Pos, unsigned _height, TTF_Font* _font);

        void render() override;
    };