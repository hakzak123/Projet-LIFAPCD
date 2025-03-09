#pragma once

struct pos{
protected:
    float x0 = 0,y0 = 0;
public:
    float x = 0,y = 0;

    pos(){

    }

    pos(float _x, float _y): x(_x), y(_y), x0(_x), y0(_y){
        
    }


    float iniX(){
        return x0;
    }

    float iniY(){
        return y0;
    }
};