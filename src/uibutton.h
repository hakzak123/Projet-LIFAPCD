#pragma once
#include <SDL3/SDL.h>
#include <uitexture.h>

class uiButton : public uiComponent{
protected:
    bool active;
    void (*onClick)(uiButton*);

public:
    uiButton(SMM* _app, pos _Pos, void (*_onClick)(uiButton*), bool _active = true) : uiComponent(_app,_Pos), onClick(_onClick), active(_active)
    {}

    virtual bool clickable() = 0;

    void action(){
        if(onClick)
            onClick(this);
    }

    bool isActive(){
        return active;
    }

    void setState(bool _active){
        _active = active;
    }
};

class uiButtonRect : public uiButton{
protected:
    fRect rect;
    color rectColor;

    void renderRect(){
        windowInfo& winfo = app->winfo;
        SDL_Renderer* renderer = app->renderer;
        float rectWidth = rect.w;
        float gap = winfo.w()-rect.x;
        float overflow = rectWidth-gap;
        bool positiveOverflow = overflow > 0 ? true : false;
        
        if(positiveOverflow){
            rect.x -= overflow;
        }
        setRenderDrawColor(app->renderer,rectColor);
        SDL_RenderFillRect(app->renderer,&rect);
    }

public:

    uiButtonRect(SMM* _app, void (*_onClick)(uiButton*), const fRect &_rect, color _rectColor, bool _active = true) : 
    uiButton(_app, _rect.getPos(), _onClick, _active),
    rect(_rect),
    rectColor(_rectColor)
    {
    }

    void setRectColor(color newColor){
        rectColor = newColor;
    }

    void setRectColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
        rectColor = color(r,g,b,a);
    }

    color getRectColor(){
        return rectColor;
    }


    bool clickable() override{
        pos mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        if(isInRectangle(mousePos, rect)){
            return true;
        }
        return false;
    }

    void update() override{
        windowInfo& winfo = app->winfo;
        SDL_Renderer* renderer = app->renderer;
        Pos.x = Pos.iniX()*winfo.w()/winfo.iniW();
        Pos.y = Pos.iniY()*winfo.h()/winfo.iniH();
        rect.setPos(Pos);
    }

    void render() override{
        renderRect();
    }

};

class uiButtonRectText : public uiButtonRect{
protected:
    uiTTFComponent text;

public:
    uiButtonRectText(SMM* _app, pos _Pos, void (*_onClick)(uiButton*), fRect _rect, color _rectColor, uiTTFComponent _text);

    void render() override;
};

class uiButtonRectTexture : public uiButtonRect{
protected:
    SDL_Texture* texture;

public:
    uiButtonRectTexture(SMM* _app, pos _Pos, void (*_onClick)(uiButton*), fRect _rect, color _rectColor, SDL_Texture* _texture);

    void render() override;
};

class uiButtonTexture : public uiButton{
protected:
    SDL_Texture* texture;
    fRect dstRect;
    
public:

    uiButtonTexture(SMM* _app, pos _Pos, void (*_onClick)(uiButton*), SDL_Texture* _texture, fRect _dstRect);
    
    bool clickable() override;

    void update() override;

    void render() override;

};