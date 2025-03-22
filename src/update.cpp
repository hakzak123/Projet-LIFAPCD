#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <ui.h>
#include <windowInfo.h>
#include <application.h>

extern ui Ui;

void SMM::update(){
    cursorHandling();
    winfo.update();
    Ui.update();
}