#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <unordered_map>
#include <color.h>
#include <ui.h>
#include <windowInfo.h>
#include <smartptr.h>
#include <ttf.h>
#include <application.h>

void debugScreenSetup(SMM*);
void mainMenuSetup(SMM*);
void statsSetup(SMM*);
void settingsSetup(SMM*);
void editorSetup(SMM*);
void loadingScreenSetup(SMM*);

void SMM::uiSetup(){
    mainMenuSetup(this);
    debugScreenSetup(this);
    statsSetup(this);
    settingsSetup(this);
    editorSetup(this);
    loadingScreenSetup(this);
}
