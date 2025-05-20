#include <application.h>

SMM* app;

int main(int argc, char* argv[]){
    app = new SMM(argc, argv, "SMM", 0, 0, SDL_WINDOW_FULLSCREEN);

    while(app->appRunning){
        auto frameStart = CURRENT_TIME;

        app->eventHandling();
        
        app->update();
        app->render();
        app->framerateHandling(frameStart);
    }

    delete app;
}