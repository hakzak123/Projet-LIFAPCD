#pragma once
#include <SDL3/SDL.h>
#include <mapcomponent.h>
#include <vector>

class tile : public mapComponent{
private :
    std::string textureName;

public :
    tile(const std::string& _textureName  = "placeholder.bmp", const pos& _Pos = pos(0,0),bool _collision = true) : 
    mapComponent(_Pos, _collision),
    textureName(_textureName)
    {}

    void setTexture(const std::string& _textureName){
        textureName = _textureName;
    }

    std::string getTextureName() const{
        return textureName;
    }
};

class tileMap{
private :
    unsigned width, height;
    std::vector<tile> tiles;

    constexpr unsigned _1DIndex(unsigned _x, unsigned _y) const{
        unsigned index = _y*width+_x;
        if(index < width*height)
            return index;
        else
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"ERROR!","A tileMap method tries to access a tile with no valid index.",nullptr);
            std::exit(1);
    }

public :
    tileMap() : width(50), height(50), tiles(width*height){}

    tileMap(unsigned short _width, unsigned short _height) : width(_width), height(_height), tiles(_width*_height)
    {}

    void newDim(unsigned short _width, unsigned short _height){
        width = _width;
        height = _height;
        tiles.resize(width*height);
    }
    tile& getTile(unsigned x, unsigned y){
        return tiles[_1DIndex(x, y)];
    }
    unsigned getWidth(){
        return width;
    }
    unsigned getHeight(){
        return height;
    }
    std::vector<tile>& getTiles(){
        return tiles;
    }

    void makeTileRectangle(unsigned Xmin,unsigned Ymin,unsigned Xmax,unsigned Ymax, const std::string& _textureName){
        if(Xmin <=Xmax && Ymin <= Ymax && Xmax < width && Ymax < height){
            for(unsigned i=Xmin; i<Xmax; ++i){
                for(unsigned j=Ymin; j<Ymax; ++j){
                    getTile(i,j).setTexture(_textureName);
                }
            }
        }
    }
};