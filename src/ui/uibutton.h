#pragma once
#include <SDL3/SDL.h>
#include <SDL3_gfx/SDL3_gfxPrimitives.h>
#include <uitexture.h>

class uiButton : public uiComponent{
protected:
    bool active;
    void (*onClick)(uiButton*);

public:
    uiButton(pos _Pos, void (*_onClick)(uiButton*), bool _active = true) : uiComponent(_Pos), onClick(_onClick), active(_active)
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

    virtual SDL_Texture* getTexture(){
        return NULL;
    }
};

class uiButtonRect : public uiButton{
protected:
    fRect rect;
    color rectColor;
    bool roundedCorners;
    bool hovering;

    void updateRect(SMM* app){
        // scaling
        const windowInfo& winfo = app->getWindowInfo();
        Pos.x = Pos.iniX()*winfo.w()/winfo.iniW();
        Pos.y = Pos.iniY()*winfo.h()/winfo.iniH();
        rect.setPos(Pos);

        rect.w = rect.iniW() / app->getWindowInfo().iniW() * winfo.w();
        rect.h = rect.iniH() / app->getWindowInfo().iniH() * winfo.h();
    }

    void renderRect(SMM* app){
        //setRenderDrawColor(app->getRenderer(),rectColor);
        //SDL_RenderFillRect(app->getRenderer(),&rect);
        if(roundedCorners){
            roundedBoxRGBA(app->getRenderer(),rect.x, rect.y, rect.x+rect.w, rect.y+rect.h, std::min(rect.w, rect.h) / 6, rectColor.r, rectColor.g, rectColor.b, rectColor.a);
        }
        else{
            setRenderDrawColor(app->getRenderer(),rectColor);
            SDL_RenderFillRect(app->getRenderer(),&rect);
        }
    }

public:

    uiButtonRect(void (*_onClick)(uiButton*), const fRect &_rect, color _rectColor, bool _roundedCorners = true, bool _active = true, bool _hovering = true) : 
    uiButton(_rect.getPos(), _onClick, _active),
    rect(_rect),
    rectColor(_rectColor),
    roundedCorners(_roundedCorners),
    hovering(_hovering)
    {
    }

    void setRect(const fRect& _rect){
        rect = _rect;
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
        if(active){
            pos mousePos;
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            if(isInRectangle(mousePos, rect)){
                return true;
            }
        }
        return false;
    }

    void update(SMM* app) override{
        updateRect(app);
    }

    void render(SMM* app) override{
        renderRect(app);
        if(clickable() && hovering){
            if(roundedCorners){
                roundedBoxRGBA(app->getRenderer(),rect.x, rect.y, rect.x+rect.w, rect.y+rect.h, std::min(rect.w, rect.h) / 6, 127, 127, 127, 100);
            }
            else{
                setRenderDrawColor(app->getRenderer(),color(127,127,127,80));
                SDL_RenderFillRect(app->getRenderer(),&rect);
            }
        }
    }

};

class uiButtonRectText : public uiButtonRect{
protected:
    uiTTFComponent text;

    void textCentering(){
        fRect tmpRect = text.getRect();
        float xMid = rect.x + rect.w/2;
        float yMid = rect.y + rect.h/2;
        tmpRect.x = xMid - tmpRect.w/2;
        tmpRect.y = yMid - tmpRect.h/2;
        text.setRect(tmpRect);
    }

public:
    uiButtonRectText(SMM* _app, void (*_onClick)(uiButton*), fRect _rect, color _rectColor, std::string _text, TTF_Font* _font, unsigned _textHeight, color _textColor, bool _roundedCorners = true, bool _active = true, bool hovering = true) : 
    uiButtonRect(_onClick,_rect,_rectColor, _roundedCorners, _active, hovering),
    text(_app, _text, calculateTextPos(_app, _text,_textHeight), _textHeight, _font, _textColor)
    {
    }

    pos calculateTextPos(SMM* app, std::string _text, unsigned _textHeight){
        updateRect(app);

        float adjustedX;
        float adjustedY;
        float xMid = rect.x + rect.w/2;
        float yMid = rect.y + rect.h/2;
        float charwidth = 0.5*_textHeight;
        float halfTextLenght;
        if(_text.size()%2 == 0){
            halfTextLenght = _text.size()/2*charwidth;
        }
        else{
            halfTextLenght = (_text.size()/2 + 0.5)*charwidth;
        }
        adjustedX = xMid-halfTextLenght;
        adjustedY = yMid - _textHeight/2;
        return pos(adjustedX,adjustedY);
    }

    void update(SMM* app) override{
        updateRect(app);
        textCentering();
    }

    void render(SMM* app) override{
        renderRect(app);
        if(clickable() && hovering){
            if(roundedCorners){
                roundedBoxRGBA(app->getRenderer(),rect.x, rect.y, rect.x+rect.w, rect.y+rect.h, std::min(rect.w, rect.h) / 6, 127, 127, 127, 100);
            }
            else{
                setRenderDrawColor(app->getRenderer(),color(127,127,127,80));
                SDL_RenderFillRect(app->getRenderer(),&rect);
            }
        }
        text.render(app);
    }
};

class uiButtonRectTexture : public uiButtonRect{
protected:
    SDL_Texture* texture;
    fRect dstRect;

    void dstRectCentering(){
        fRect tmpRect = dstRect;
        float xMid = rect.x + rect.w/2;
        float yMid = rect.y + rect.h/2;
        tmpRect.x = xMid - tmpRect.w/2;
        tmpRect.y = yMid - tmpRect.h/2;
        dstRect = tmpRect;
    }
public:
    uiButtonRectTexture( void (*_onClick)(uiButton*), fRect _rect, color _rectColor, SDL_Texture* _texture, fRect _dstRect, bool _roundedCorners = true, bool _active = true, bool hovering = true) : 
    uiButtonRect(_onClick, _rect, _rectColor, _roundedCorners, _active, hovering),
    texture(_texture),
    dstRect(_dstRect)
    {}

    void update(SMM* app) override{
        updateRect(app);
        dstRectCentering();
    }

    SDL_Texture* getTexture(){
        return texture;
    }

    void render(SMM* app) override{
        renderRect(app);
        SDL_RenderTexture(app->getRenderer(),texture,NULL,&dstRect);
        if(clickable()){
            if(roundedCorners){
                roundedBoxRGBA(app->getRenderer(),rect.x, rect.y, rect.x+rect.w, rect.y+rect.h, std::min(rect.w, rect.h) / 6, 127, 127, 127, 100);
            }
            else{
                SDL_SetRenderDrawBlendMode(app->getRenderer(), SDL_BLENDMODE_BLEND);
                setRenderDrawColor(app->getRenderer(),color(127,127,127,80));
                SDL_RenderFillRect(app->getRenderer(),&rect);
                
            }
        }    
    }
};

class uiButtonRectTextureScaled : public uiButtonRectTexture{

    public :

    uiButtonRectTextureScaled(void (*_onClick)(uiButton*), fRect _rect, color _rectColor, SDL_Texture* _texture, fRect _dstRect, bool _roundedCorners = true, bool _active = true, bool hovering = true) : 
    uiButtonRectTexture(_onClick, _rect, _rectColor, _texture, _dstRect, _roundedCorners, _active, hovering)
    {}

    void render(SMM* app) override{
        renderRect(app);
        SDL_RenderTexture(app->getRenderer(),texture,NULL,&rect);
        if(clickable() && hovering){
            if(roundedCorners){
                roundedBoxRGBA(app->getRenderer(),rect.x, rect.y, rect.x+rect.w, rect.y+rect.h, std::min(rect.w, rect.h) / 6, 127, 127, 127, 100);
            }
            else{
                SDL_SetRenderDrawBlendMode(app->getRenderer(), SDL_BLENDMODE_BLEND);
                setRenderDrawColor(app->getRenderer(),color(127,127,127,80));
                SDL_RenderFillRect(app->getRenderer(),&rect);
                
            }
        }    
    }
};