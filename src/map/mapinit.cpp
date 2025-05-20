#include <map.h>

map g_map(nullptr, tileMap(), spawnPoint());

void SMM::mapInit(){
    g_map = map(this, tileMap(), spawnPoint());
    g_map.init();
    g_map.setRenderTarget(fRect(0,0,0,0));
}