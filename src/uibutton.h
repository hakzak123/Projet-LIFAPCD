#pragma once
#include <SDL3/SDL.h>
#include <uitexture.h>

class uiButton : public uiComponent{
protected:
    bool active;
    void (*onClick)();
public:
    uiButton(SMM* _app, pos _Pos, void (*_onClick)());

    virtual bool clickable() = 0;

    void action(){
        onClick();
    }

    bool isActive();

    void setState(bool _active);
};

class uiButtonRect : public uiButton{
protected:
    fRect rect;
    color rectColor;
public:
    uiButtonRect(SMM* _app, pos _Pos, void (*_onClick)(), fRect _rect, color _rectColor);

    void setRectColor(color newColor);

    color getRectColor();

    void renderRect();

    bool clickable() override;

    void update() override;

    void render() override;

};

class uiButtonRectText : public uiButtonRect{
protected:
    uiTTFComponent text;

public:
    uiButtonRectText(SMM* _app, pos _Pos, void (*_onClick)(), fRect _rect, color _rectColor, uiTTFComponent _text);

    void render() override;
};

class uiButtonRectTexture : public uiButtonRect{
protected:
    SDL_Texture* texture;

public:
    uiButtonRectTexture(SMM* _app, pos _Pos, void (*_onClick)(), fRect _rect, color _rectColor, SDL_Texture* _texture);

    void render() override;
};

class uiButtonTexture : public uiButton{
protected:
    SDL_Texture* texture;
    fRect dstRect;
    
public:

    uiButtonTexture(SMM* _app, pos _Pos, void (*_onClick)(), SDL_Texture* _texture, fRect _dstRect);
    
    bool clickable() override;

    void update() override;

    void render() override;

};