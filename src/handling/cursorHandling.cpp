#include <SDL3/SDL.h>
#include <ui.h>

void SMM::cursorHandling(){
    if(!arrowCursor)
        arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    if(!handCursor)
        handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
    for(auto& e : getUi()){
        if(e.second->isEnabled())
            switch(e.second->cursorHandler()){
                case SDL_SYSTEM_CURSOR_DEFAULT : {
                    SDL_SetCursor(arrowCursor);
                    goto endloop;
                }
                case SDL_SYSTEM_CURSOR_POINTER : {
                    SDL_SetCursor(handCursor);
                    goto endloop;
                }
            }
    }
    endloop: ;
}