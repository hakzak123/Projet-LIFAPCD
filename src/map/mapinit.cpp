#include <map.h>

map g_map(nullptr, tileMap(), spawnPoint());

void mapInit(SMM* _app){
    g_map = map(_app, tileMap(), spawnPoint());
    g_map.init();
    g_map.setRenderTarget(fRect(0,0,0,0));
}