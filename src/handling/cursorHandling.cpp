#include <SDL3/SDL.h>
#include <ui.h>

extern ui Ui;

void SMM::cursorHandling(){
    if(!arrowCursor)
        arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    if(!handCursor)
        handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
    switch(Ui.cursorHandler()){
        case SDL_SYSTEM_CURSOR_DEFAULT : {
            SDL_SetCursor(arrowCursor);
            break;
        }
        case SDL_SYSTEM_CURSOR_POINTER : {
            SDL_SetCursor(handCursor);
            break;
        }
    }
}