#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <map>
#include <color.h>
#include <windowInfo.h>
#include <position.h>
#include <geometry.h>
#include <smartptr.h>


class uiTextComponent{ // corrects a debug text's position so that it doesn't overflow to the right.
protected :

    SDL_Renderer* renderer;
    std::string text;
    color stringColor;
    float x,y;
    pos iniPos;
    windowInfo iniWinfo;
    windowInfo* winfo;
    int charwidth = 8;
    bool rendered = true;

public:

    uiTextComponent(SDL_Renderer* _renderer,const std::string& _text, pos _position, const color &_stringColor,windowInfo* _winfo) : 
    renderer(_renderer), 
    text(_text),
    x(_position.x), 
    y(_position.y), 
    winfo(_winfo), 
    stringColor(_stringColor),
    iniWinfo(*_winfo),
    iniPos(_position)
   {}

    bool isRendered(){
        return rendered;
    }

    void renderable(bool b){
        rendered = b;
    }

    void update(){
        x = iniPos.x*winfo->w/iniWinfo.w;
        y = iniPos.y*winfo->h/iniWinfo.h;
    }

    virtual void render(){
        if(rendered){
            size_t stringsize = text.size()*charwidth;
            int width = winfo->w;
            float gap = width-x;
            float overflow = stringsize-gap;
            bool positiveOverflow = overflow > 0 ? true : false;

            setRenderDrawColor(renderer,stringColor);
            
            if(positiveOverflow){
                SDL_RenderDebugText(renderer,x-overflow,y,text.c_str());
            }
            else{ 
                SDL_RenderDebugText(renderer,x,y,text.c_str());
            }
        }
    }
};

class uiTextureComponent{ // corrects a texture's rectangle position so that it doesn't overflow to the right. Doesn't take in src rectangle for simplicity.
protected :     
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    windowInfo* winfo;
    fRect dstRect;
    fRect iniRect;
    windowInfo iniWinfo;
    bool rendered = true;

public :

    uiTextureComponent(SDL_Renderer* _renderer, SDL_Texture* _texture, windowInfo* _winfo,fRect _dstRect):
    renderer(_renderer),
    texture(_texture),
    winfo(_winfo),
    dstRect(_dstRect),
    iniRect(_dstRect),
    iniWinfo(*_winfo)
    {
    }

    bool isRendered(){
        return rendered;
    }

    void renderable(bool b){
        rendered = b;
    }

    void update(){
        dstRect.x = iniRect.x*winfo->w/iniWinfo.w;
        dstRect.y = iniRect.y*winfo->h/iniWinfo.h;
    }

    void render(){
        if(rendered){
            fRect tmpRect = dstRect;
            size_t rectWidth = dstRect.w;
            int width = winfo->w;
            float gap = width-dstRect.x;
            float overflow = rectWidth-gap;
            bool positiveOverflow = overflow > 0 ? true : false;
            
            if(positiveOverflow){
                tmpRect.x -= overflow;
            }
            SDL_RenderTexture(renderer,texture,NULL,&tmpRect);
        }
    }

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
                x = positionptr().x;
                y = positionptr().y;
            }
            if(stringcolorptr)
                stringColor = stringcolorptr();

            size_t stringsize = text.size()*charwidth;
            int width = winfo->w;
            float gap = width-x;
            float overflow = stringsize-gap;
            bool positiveOverflow = overflow > 0 ? true : false;

            setRenderDrawColor(renderer,stringColor);
            
            if(positiveOverflow){
                SDL_RenderDebugText(renderer,x-overflow,y,text.c_str());
            }
            else{ 
                SDL_RenderDebugText(renderer,x,y,text.c_str());
            }
        }
    }

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