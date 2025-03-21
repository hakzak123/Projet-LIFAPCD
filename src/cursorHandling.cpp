#include <SDL3/SDL.h>
#include <ui.h>

extern ui Ui;
SDL_Cursor* handCursor;
SDL_Cursor* arrowCursor;

void cursorHandling(){
    arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
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

void destroyCursors(){
    SDL_DestroyCursor(handCursor);
    SDL_DestroyCursor(arrowCursor);
}