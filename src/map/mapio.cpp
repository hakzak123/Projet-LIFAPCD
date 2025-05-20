#include <map.h>

extern map g_map;
/*
smp format :
spawnpoint
tilesize
tileMap : uint32 width, height, uint16 texturemap size, array of textures (uint16, C-string), array of tiles (bool + C-string)
*/

int SMM::saveMapToFile(const char* filePath){
    Uint64 timeBefore = SDL_GetTicks();

    int index = 0;
    size_t sizeOfFile = 0;
    size_t offset = 0;
    char* file = nullptr;
    tileMap& tile_map = g_map.getTileMap();

    sizeOfFile += sizeof(spawnPoint);
    sizeOfFile += sizeof(unsigned)*3; // tilesize, width, height
    
    sizeOfFile += sizeof(Uint16)*getGlobalTextures().size();
    for(const auto& texture : getGlobalTextures()){
        sizeOfFile += texture.first.size() + 1;
    }

    sizeOfFile += sizeof(Uint16);
    sizeOfFile += sizeof(bool)*tile_map.getTiles().size();
    sizeOfFile += sizeof(Uint16)*tile_map.getTiles().size();

    file = (char*)SDL_malloc(sizeOfFile); // verifier que l'alloc a réussi

    *(spawnPoint*)file = g_map.getSpawnPoint();
    offset += sizeof(spawnPoint);

    *(unsigned*)(file + offset) = g_map.getTileSize();
    offset += sizeof(unsigned);

    *(unsigned*)(file + offset) = tile_map.getWidth();
    offset += sizeof(unsigned);
    *(unsigned*)(file + offset) = tile_map.getHeight();
    offset += sizeof(unsigned);

    *(Uint16*)(file + offset) = getGlobalTextures().size();
    offset += sizeof(Uint16);

    std::map<std::string, Uint16> textureDescMap;

    for(const auto& texture : getGlobalTextures()){
        textureDescMap[texture.first] = index;
        *(Uint16*)(file + offset) = index;
        offset += sizeof(Uint16);

        memcpy(file+offset, texture.first.c_str(), texture.first.size()+1);
        offset += texture.first.size()+1;

        index++;
    }

    index = 0;

    for(const auto e : tile_map.getTiles()){
        *(bool*)(file + offset) = e.getCollision();
        offset += sizeof(bool);

        *(Uint16*)(file + offset) = textureDescMap[e.getTextureName()];
        offset += sizeof(Uint16);

        index++;
    }

    SDL_SaveFile(filePath,file,sizeOfFile);

    Uint64 saveTime = SDL_GetTicks() - timeBefore;

    printf("Saved map to %s, took %u ms\n", filePath, saveTime);

    return 0;
}

int loadMapFromFile(const char* filePath){ // empêcher de quitter ou faire des modifications quand une map est en train d'être sauvegardée
    size_t fileSize;
    size_t offset = 0;
    int index = 0;
    tileMap& tile_map = g_map.getTileMap();

    if(filePath == nullptr){
        return -1;
    }

    char* file = (char*)SDL_LoadFile(filePath,&fileSize);
    
    spawnPoint spawn = *(spawnPoint*)file;
    offset += sizeof(spawnPoint);
    g_map.setSpawnPoint(spawn);
    
    unsigned tileSize = *(unsigned*)(file + offset);
    offset += sizeof(unsigned);
    g_map.setTileSize(tileSize);
    
    unsigned width, height;
    width = *(unsigned*)(file + offset);
    offset += sizeof(unsigned);
    height = *(unsigned*)(file + offset);
    offset += sizeof(unsigned);
    tile_map.newDim(width,height);

    Uint16 textureCount = *(Uint16*)(file + offset);
    offset += sizeof(Uint16);

    std::map<Uint16, std::string> descTextureMap;

    for(int i = 0; i < textureCount; ++i){
        Uint16 descriptor = *(Uint16*)(file + offset);
        offset += sizeof(Uint16);

        std::string textureName = (const char*)(file + offset);
        offset += textureName.size() + 1;
        descTextureMap[descriptor] = textureName;
    }

    while(offset < fileSize){
        mapComponent m;

        bool collisions = *(bool*)(file + offset);
        offset += sizeof(bool);
        m.setCollision(collisions);

        Uint16 descriptor = *(Uint16*)(file + offset);
        offset += sizeof(Uint16);
        std::string texture = descTextureMap[descriptor];

        tile t(texture,m.getPos(),m.getCollision());
        tile_map.getTiles()[index] = t;

        index++;
    }

    printf("Map loaded. Map dimensions : %ux%u, amount of tiles : %u\n",tile_map.getWidth(), tile_map.getHeight(), tile_map.getTiles().size());

    SDL_free(file);
    return 0;
}