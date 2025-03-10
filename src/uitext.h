#pragma once
#include <SDL3/SDL.h>
#include <uicomp.h>
#include <color.h>


class uiTextComponent : public uiComponent{ // corrects a debug text's position so that it doesn't overflow to the right.
protected :
        std::string text;
        color stringColor;
    
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
                size_t stringsize = text.size()*8;
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
    
                size_t stringsize = text.size()*8;
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