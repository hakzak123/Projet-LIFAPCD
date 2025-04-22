#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <map>
#include <uicomp.h>
#include <uitext.h>
#include <uitexture.h>
#include <uibutton.h>
#include <macros.h>


class screen{ // Stores ui component maps
private :
    std::map<std::string,uiComponent*> uiCompMap;
    bool enabled = true;

public :
    screen(){}

    std::map<std::string,uiComponent*>& getUiComponents(){
        return uiCompMap;
    }


    auto& operator[](std::string s){
        return uiCompMap[s];
    }

    bool isEnabled(){
        return enabled;
    }

    void setEnabled(bool b){
        enabled = b;
    }

    void update(SMM* app){
        if(enabled)
            for(auto& e : uiCompMap){
                e.second->update(app);
            }
    }

    void render(SMM* app){
        if(enabled)
            for(auto& e : uiCompMap){
                e.second->render(app);
            }
    }

    void eventHandler(const SDL_Event& event){
        if(enabled)
            switch(event.type){
                case SDL_EVENT_MOUSE_BUTTON_UP: {
                    uiButton* button;
                    if(event.button.button == MOUSE_LEFT_CLICK)
                        for(auto& e : uiCompMap){
                            if(button = dynamic_cast<uiButton*>(e.second)){
                                if(button->clickable() && enabled){
                                    button->action();
                                }
                            }
                        }
                    break;
                }
            }
    }

    SDL_SystemCursor cursorHandler(){ // return the correct SDL_SystemCursor
        if(enabled)
            for(auto& e : uiCompMap){
                uiButton* button;
                if(button = dynamic_cast<uiButton*>(e.second)){
                    if(button->clickable() && button->isRendered()){
                        return SDL_SYSTEM_CURSOR_POINTER;
                    }
                }
            }
        return SDL_SYSTEM_CURSOR_DEFAULT;
    }

    ~screen(){
        for(auto& e : uiCompMap){
            delete e.second;
        }
        delete this;
    }

};