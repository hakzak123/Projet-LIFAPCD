#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <ui.h>
#include <windowInfo.h>
#include <application.h>

extern ui Ui;

void update(SMM* app){
    app->winfo.update();
    Ui.update();
}