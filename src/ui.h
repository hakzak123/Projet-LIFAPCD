#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <map>
#include <uicomp.h>
#include <uitext.h>
#include <uitexture.h>
#include <uibutton.h>


class ui{ // Stores ui component maps
public :
    std::map<std::string,uiComponent*> uiCompMap;

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

    ~ui(){
        for(auto& e : uiCompMap){
            delete e.second;
        }
    }

};