#pragma once
#include <SDL3/SDL.h>
#include <map.h>

class tile : public mapComponent{
private :
    std::string textureName = "placeholder.bmp";

public :
    tile();
    tile(std::string _textureName, bool _collision = true);
};

class tileMap{
private :
    unsigned short width, height;
    tile* tiles = nullptr;

public :
    tileMap(unsigned short _width, unsigned short _height) : width(_width), height(_height), tiles(new tile[int(width)*height])
    {}

    tileMap(const tileMap& other);

    void operator=(const tileMap& other);

    ~tileMap();

    void newDim(unsigned short _width, unsigned short _height);
    tile& getTile();
    void setTile(unsigned short x, unsigned short y);
    void makeTileRectangle(unsigned short _width, unsigned short _height, std::string textureName);

};