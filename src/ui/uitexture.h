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

    uiTextureComponent(SDL_Texture* _texture,fRect _dstRect):
    uiComponent(pos(_dstRect.x,_dstRect.y)),
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

    void update(SMM* app) override{
        const windowInfo& winfo = app->getWindowInfo();
        SDL_Renderer* renderer = app->getRenderer();
        Pos.x = Pos.iniX()*winfo.w()/winfo.iniW();
        Pos.y = Pos.iniY()*winfo.h()/winfo.iniH();
        dstRect.setPos(Pos);
    }

    void render(SMM* app) override{
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

    uiTTFComponent(SMM* _app, std::string _text, pos _Pos, unsigned _height, TTF_Font* _font, color _textColor)
    : uiTextureComponent(createTTFTexture(_app->getRenderer(), _font, _text, _textColor), calculateTextRect(_font, _text, _Pos, _height))
    {
    }

    static fRect calculateTextRect(TTF_Font* font, const std::string& text, pos position, unsigned height) {
        int textWidth, textHeight;
        TTF_GetStringSize(font, text.c_str(), 0, &textWidth, &textHeight);
        float scale = static_cast<float>(height) / textHeight;
        return fRect(position.x, position.y, textWidth * scale, height);
    }

    ~uiTTFComponent(){
        SDL_DestroyTexture(texture);
    }


    // pas de shallow copy sinon copy d'une texture détruite
};