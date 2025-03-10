#pragma once
#include <SDL3/SDL.h>
#include <uicomp.h>
#include <color.h>


class uiTextComponent : public uiComponent{ // corrects a debug text's position so that it doesn't overflow to the right.
protected :
        std::string text;
        color stringColor;
    
public:
        uiTextComponent(SMM* _app,const std::string& _text, pos _position, const color &_stringColor) : 
        uiComponent(_app,_position),
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
        uiDynamicTextComponent(SMM* _app,std::string (*_textptr)(), pos (*_positionptr)(), color (* _stringcolorptr)()) : 
        uiTextComponent(_app,_textptr(),_positionptr(),_stringcolorptr()),
        positionptr(_positionptr),
        textptr(_textptr),
        stringcolorptr(_stringcolorptr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string _text, pos (*_positionptr)(), color (* _stringcolorptr)()) : 
        uiTextComponent(_app,_text,_positionptr(),_stringcolorptr()),
        positionptr(_positionptr),
        textptr(nullptr),
        stringcolorptr(_stringcolorptr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string (*_textptr)(), pos _position, color (* _stringcolorptr)()) : 
        uiTextComponent(_app,_textptr(),_position,_stringcolorptr()),
        positionptr(nullptr),
        textptr(_textptr),
        stringcolorptr(_stringcolorptr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string (*_textptr)(), pos (*_positionptr)(), color  _stringcolor) : 
        uiTextComponent(_app,_textptr(),_positionptr(),_stringcolor),
        positionptr(_positionptr),
        textptr(_textptr),
        stringcolorptr(nullptr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string _text, pos _position, color (* _stringcolorptr)()) : 
        uiTextComponent(_app,_text,_position,_stringcolorptr()),
        positionptr(nullptr),
        textptr(nullptr),
        stringcolorptr(_stringcolorptr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string (*_textptr)(), pos _position, color _stringcolor) : 
        uiTextComponent(_app,_textptr(),_position,_stringcolor),
        positionptr(nullptr),
        textptr(_textptr),
        stringcolorptr(nullptr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string _text, pos (*_positionptr)(), color _stringcolor) : 
        uiTextComponent(_app,_text,_positionptr(),_stringcolor),
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