#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <map>
#include <uicomp.h>
#include <uitext.h>
#include <uitexture.h>
#include <uibutton.h>
#include <macros.h>


class ui{ // Stores ui component maps
private :
    std::map<std::string,uiComponent*> uiCompMap;

public :
    ui(){
    }

    void insert(std::string key, uiComponent* comp){
        uiCompMap.insert({key,comp});
    }

    void update(){
        for(auto& e : uiCompMap){
            e.second->update();
        }
    }

    void render(){
        for(auto& e : uiCompMap){
            e.second->render();
        }
    }

    void eventHandler(const SDL_Event& event){
        switch(event.type){
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                uiButton* button;
                if(event.button.button == MOUSE_LEFT_CLICK)
                    for(auto& e : uiCompMap){
                        if(button = dynamic_cast<uiButton*>(e.second)){
                            if(button->clickable()){
                                button->action();
                            }
                        }
                    }
                break;
            }
        }
    }

    SDL_SystemCursor cursorHandler(){ // return the correct SDL_SystemCursor
        for(auto& e : uiCompMap){
            uiButton* button;
            if(button = dynamic_cast<uiButton*>(e.second)){
                if(button->clickable()){
                    return SDL_SYSTEM_CURSOR_POINTER;
                }
            }
        }
        return SDL_SYSTEM_CURSOR_DEFAULT;
    }

    ~ui(){
        for(auto& e : uiCompMap){
            delete e.second;
        }
    }

};