#pragma once
#include <SDL3/SDL.h>
#include <map.h>

class tile : public mapComponent{
private :
    std::string textureName = "placeholder.bmp";
    bool collisions = true;

public :
    tile();

    tile(std::string _textureName, bool _collisions = true);
};

class tileMap{
private :
    unsigned short width, height;
    tile* tiles = nullptr;

    void reAlloc(unsigned short _width, unsigned short _height);

public :
    tileMap(unsigned short _width, unsigned short _height) : width(_width), height(_height), tiles(new tile[int(width)*height])
    {}

    tileMap(const tileMap& other);

    void operator=(const tileMap& other);

    ~tileMap();

    void newDim(unsigned short _width, unsigned short _height);
    tile& getTile();
    tile getTile() const;
    void setTile(unsigned short x, unsigned short y);

};