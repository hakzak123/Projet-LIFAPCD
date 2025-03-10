#pragma once
#include <SDL3/SDL.h>
#include <uitexture.h>

class uiButton : public uiComponent{
    protected:
        bool active;
    
    public:
        virtual bool clickable() = 0;
    
        virtual void onClick(){
        }
    };
    
    class uiButtonRectBackgrnd : public uiButton{
    protected:
        fRect rectangle;
    public:
        bool clickable() override;
    
        void update() override;
    
        void render() override;
    
    };
    
    class uiButtonRectText : public uiButtonRectBackgrnd{
    protected:
        uiTTFComponent text;
    
    public:
    
        void render() override;
    };
    
    class uiButtonRectTexture : public uiButtonRectBackgrnd{
    protected:
        SDL_Texture* texture;    
    public:
        void render() override;
    };
    
    class uiButtonTexture : public uiButton{
    protected:
        SDL_Texture* texture; 
    public:
        bool clickable() override;
    
        void update() override;
    
        void render() override;
    
    };