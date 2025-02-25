#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <ui.h>
#include <windowInfo.h>

extern windowInfo winfo;
extern ui Ui;

void update(){
    winfo.update();
    Ui.update();
}