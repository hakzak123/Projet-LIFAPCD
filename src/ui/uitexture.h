#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <uicomp.h>
#include <geometry.h>
#include <ttf.h>


class uiTextureComponent : public uiComponent{ // corrects a texture's rectangle position so that it doesn't overflow to the right. Doesn't take in src rectangle for simplicity.
protected :
    SDL_Texture* texture = nullptr;
    fRect dstRect;

public :
    uiTextureComponent(){
        
    }

    uiTextureComponent(SMM* _app, SDL_Texture* _texture,fRect _dstRect):
    uiComponent(_app,pos(_dstRect.x,_dstRect.y)),
    texture(_texture),
    dstRect(_dstRect)
    {
    }

    fRect getRect(){
        return dstRect;
    }

    void setRect(const fRect& other){
        dstRect = other;
    }

    void update() override{
        const windowInfo& winfo = app->getWindowInfo();
        SDL_Renderer* renderer = app->getRenderer();
        Pos.x = Pos.iniX()*winfo.w()/winfo.iniW();
        Pos.y = Pos.iniY()*winfo.h()/winfo.iniH();
        dstRect.setPos(Pos);
    }

    void render() override{
        if(rendered){
            const windowInfo& winfo = app->getWindowInfo();
            SDL_Renderer* renderer = app->getRenderer();
            fRect tmpRect = dstRect;
            float& rectWidth = dstRect.w;
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
    public:
        uiTTFComponent(){

        }

        uiTTFComponent(SMM* _app, std::string _text ,pos _Pos, unsigned _height, TTF_Font* _font, color _textColor) : 
        uiTextureComponent(_app, createTTFTexture(_app->getRenderer(),_font,_text,_textColor),fRect(_Pos.x,_Pos.y,0.5*_height*_text.size(),_height))
        {
        }

        ~uiTTFComponent(){
            SDL_DestroyTexture(texture);
        }


        // pas de shallow copy sinon copy d'une texture détruite
    };