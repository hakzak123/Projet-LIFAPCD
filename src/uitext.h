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
            windowInfo winfo = app->winfo;
            Pos.x = Pos.iniX()*winfo.w()/winfo.iniW();
            Pos.y = Pos.iniY()*winfo.h()/winfo.iniH();
        }
    
        void render() override{
            if(rendered){
                windowInfo winfo = app->winfo;
                SDL_Renderer* renderer = app->renderer;
                size_t stringsize = text.size()*8;
                float gap = winfo.w()-Pos.x;
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
        pos (* positionPtr)() = nullptr;
        std::string (* textPtr)() = nullptr;
        color (* stringColorPtr)() = nullptr;
    
    
    public : 
        uiDynamicTextComponent(SMM* _app,std::string (*_textPtr)(), pos (*_positionPtr)(), color (* _stringColorPtr)()) : 
        uiTextComponent(_app,_textPtr(),_positionPtr(),_stringColorPtr()),
        positionPtr(_positionPtr),
        textPtr(_textPtr),
        stringColorPtr(_stringColorPtr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string _text, pos (*_positionPtr)(), color (* _stringColorPtr)()) : 
        uiTextComponent(_app,_text,_positionPtr(),_stringColorPtr()),
        positionPtr(_positionPtr),
        textPtr(nullptr),
        stringColorPtr(_stringColorPtr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string (*_textPtr)(), pos _position, color (* _stringColorPtr)()) : 
        uiTextComponent(_app,_textPtr(),_position,_stringColorPtr()),
        positionPtr(nullptr),
        textPtr(_textPtr),
        stringColorPtr(_stringColorPtr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string (*_textPtr)(), pos (*_positionPtr)(), color  _stringcolor) : 
        uiTextComponent(_app,_textPtr(),_positionPtr(),_stringcolor),
        positionPtr(_positionPtr),
        textPtr(_textPtr),
        stringColorPtr(nullptr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string _text, pos _position, color (* _stringColorPtr)()) : 
        uiTextComponent(_app,_text,_position,_stringColorPtr()),
        positionPtr(nullptr),
        textPtr(nullptr),
        stringColorPtr(_stringColorPtr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string (*_textPtr)(), pos _position, color _stringcolor) : 
        uiTextComponent(_app,_textPtr(),_position,_stringcolor),
        positionPtr(nullptr),
        textPtr(_textPtr),
        stringColorPtr(nullptr)
        {}
    
        uiDynamicTextComponent(SMM* _app,std::string _text, pos (*_positionPtr)(), color _stringcolor) : 
        uiTextComponent(_app,_text,_positionPtr(),_stringcolor),
        positionPtr(_positionPtr),
        textPtr(nullptr),
        stringColorPtr(nullptr)
        {}
    
        void render() override {
            if(rendered){
                windowInfo winfo = app->winfo;
                SDL_Renderer* renderer = app->renderer;
                if(textPtr)
                    text = textPtr();           
                if(positionPtr){
                    Pos.x = positionPtr().x;
                    Pos.y = positionPtr().y;
                }
                if(stringColorPtr)
                    stringColor = stringColorPtr();
    
                size_t stringsize = text.size()*8;
                float gap = winfo.w()-Pos.x;
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