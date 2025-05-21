#include <ui.h>

void loadingScreenSetup(SMM* _app){
    SDL_Texture* textTexture = createTTFTexture(_app->getRenderer(),_app->getFont("impact.ttf"),"TEST TEST TEST",{0,255,0,255});
    const int& width = _app->getWindowInfo().w();
    const int& height = _app->getWindowInfo().h();
    screen* loadingScreen = new screen;

    uiTTFComponent* uiLoading = new uiTTFComponent(_app, "Loading...", pos(width/2-width/12,height/3), 100, _app->getFont("impact.ttf"), color(255,255,255,255));

    (*loadingScreen)["loading"] = uiLoading;
    _app->insertScreen("loading", loadingScreen);
    loadingScreen->setEnabled(false);
}