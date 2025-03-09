#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <map>
#include <color.h>
#include <windowInfo.h>
#include <position.h>
#include <geometry.h>
#include <smartptr.h>

class uiComponent{
protected :
    SDL_Renderer* renderer;
    windowInfo* winfo;
    pos Pos;
    bool rendered = true;
public :
    uiComponent(SDL_Renderer* _renderer, windowInfo* _winfo, pos _Pos) : renderer(_renderer), winfo(_winfo), Pos(_Pos){

    }

    bool isRendered(){
        return rendered;
    }

    void renderable(bool b){
        rendered = b;
    }
    virtual void update() = 0;
    virtual void render() = 0;
};

class uiTextComponent : public uiComponent{ // corrects a debug text's position so that it doesn't overflow to the right.
protected :

    std::string text;
    color stringColor;
    const int charwidth = 8;


public:

    uiTextComponent(SDL_Renderer* _renderer,const std::string& _text, pos _position, const color &_stringColor,windowInfo* _winfo) : 
    uiComponent(_renderer, _winfo,_position),
    text(_text),
    stringColor(_stringColor)
    {}


    void update() override{
        Pos.x = Pos.iniX()*winfo->w()/winfo->iniW();
        Pos.y = Pos.iniY()*winfo->h()/winfo->iniH();
    }

    void render() override{
        if(rendered){
            size_t stringsize = text.size()*charwidth;
            float gap = winfo->w()-Pos.x;
            float overflow = stringsize-gap;
            bool positiveOverflow = overflow > 0 ? true : false;

            setRenderDrawColor(renderer,stringColor);
            
            if(positiveOverflow){
                SDL_RenderDebugText(renderer,Pos.x-overflow,Pos.y,text.c_str());
            }
            else{ 
                SDL_RenderDebugText(renderer,Pos.x,Pos.y,text.c_str());
            }
        }
    }
};

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

class uiTTFComponent : public uiTextComponent{

};

class uiDynamicTextComponent : public uiTextComponent{ // same job as uiTextComponent but it takes AT LEAST ONE function pointer (if none use uiTextComponent)

private : 
    pos (* positionptr)() = nullptr;
    std::string (* textptr)() = nullptr;
    color (* stringcolorptr)() = nullptr;


public : 
    uiDynamicTextComponent(SDL_Renderer* _renderer,std::string (*_textptr)(), pos (*_positionptr)(), color (* _stringcolorptr)(),windowInfo* _winfo) : 
    uiTextComponent(_renderer,_textptr(),_positionptr(),_stringcolorptr(),_winfo),
    positionptr(_positionptr),
    textptr(_textptr),
    stringcolorptr(_stringcolorptr)
    {}

    uiDynamicTextComponent(SDL_Renderer* _renderer,std::string _text, pos (*_positionptr)(), color (* _stringcolorptr)(),windowInfo* _winfo) : 
    uiTextComponent(_renderer,_text,_positionptr(),_stringcolorptr(),_winfo),
    positionptr(_positionptr),
    textptr(nullptr),
    stringcolorptr(_stringcolorptr)
    {}

    uiDynamicTextComponent(SDL_Renderer* _renderer,std::string (*_textptr)(), pos _position, color (* _stringcolorptr)(),windowInfo* _winfo) : 
    uiTextComponent(_renderer,_textptr(),_position,_stringcolorptr(),_winfo),
    positionptr(nullptr),
    textptr(_textptr),
    stringcolorptr(_stringcolorptr)
    {}

    uiDynamicTextComponent(SDL_Renderer* _renderer,std::string (*_textptr)(), pos (*_positionptr)(), color  _stringcolor,windowInfo* _winfo) : 
    uiTextComponent(_renderer,_textptr(),_positionptr(),_stringcolor,_winfo),
    positionptr(_positionptr),
    textptr(_textptr),
    stringcolorptr(nullptr)
    {}

    uiDynamicTextComponent(SDL_Renderer* _renderer,std::string _text, pos _position, color (* _stringcolorptr)(),windowInfo* _winfo) : 
    uiTextComponent(_renderer,_text,_position,_stringcolorptr(),_winfo),
    positionptr(nullptr),
    textptr(nullptr),
    stringcolorptr(_stringcolorptr)
    {}

    uiDynamicTextComponent(SDL_Renderer* _renderer,std::string (*_textptr)(), pos _position, color _stringcolor,windowInfo* _winfo) : 
    uiTextComponent(_renderer,_textptr(),_position,_stringcolor,_winfo),
    positionptr(nullptr),
    textptr(_textptr),
    stringcolorptr(nullptr)
    {}

    uiDynamicTextComponent(SDL_Renderer* _renderer,std::string _text, pos (*_positionptr)(), color _stringcolor,windowInfo* _winfo) : 
    uiTextComponent(_renderer,_text,_positionptr(),_stringcolor,_winfo),
    positionptr(_positionptr),
    textptr(nullptr),
    stringcolorptr(nullptr)
    {}

    void render() override {
        if(rendered){
            if(textptr)
                text = textptr();           
            if(positionptr){
                Pos.x = positionptr().x;
                Pos.y = positionptr().y;
            }
            if(stringcolorptr)
                stringColor = stringcolorptr();

            size_t stringsize = text.size()*charwidth;
            float gap = winfo->w()-Pos.x;
            float overflow = stringsize-gap;
            bool positiveOverflow = overflow > 0 ? true : false;

            setRenderDrawColor(renderer,stringColor);
            
            if(positiveOverflow){
                SDL_RenderDebugText(renderer,Pos.x-overflow,Pos.y,text.c_str());
            }
            else{ 
                SDL_RenderDebugText(renderer,Pos.x,Pos.y,text.c_str());
            }
        }
    }

};



class uiButtonRectBackgrnd{
private:
    fRect rectangle;
    bool clickable;
public:

};

class uiButtonRectText : public uiButtonRectBackgrnd{

};

class uiButtonRectImage : public uiButtonRectBackgrnd{

};

class uiButtonImage : public uiTextureComponent{

};

class ui{ // Stores ui component maps
public :
    std::map<std::string,uiTextComponent> uiTCMap;
    std::map<std::string,uiDynamicTextComponent> uiDTCMap;
    std::map<std::string,uiTextureComponent> uiTextureCMap;

    void insert(std::string key, uiTextComponent uiComponent){
        uiTCMap.insert({key,uiComponent});
    }

    void insert(std::string key, uiDynamicTextComponent uiComponent){
        uiDTCMap.insert({key,uiComponent});
    }

    void insert(std::string key, uiTextureComponent uiComponent){
        uiTextureCMap.insert({key,uiComponent});
    }

    void update(){
        for(auto& e : uiTCMap){
            e.second.update();
        }
        for(auto& e : uiDTCMap){
            e.second.update();
        }
    }

    void render(){
        for(auto& e : uiTCMap){
            e.second.render();
        }
        for(auto& e : uiDTCMap){
            e.second.render();
        }
        for(auto& e : uiTextureCMap){
            e.second.render();
        }
    }

};