#include <application.h>

SMM* app;

int main(int argc, char* argv[]){
    app = new SMM(argc, argv, "SMM", 0, 0, SDL_WINDOW_FULLSCREEN);
    
    app->runLoop();

    delete app;
}