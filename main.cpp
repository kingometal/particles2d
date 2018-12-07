#include "particles.h"
#include "constants.h"
#include "particlesview.h"
#include "particlesviewsdladapter.h"
#include "plugin/DemoPresenter.h"
#include "plugin/UserInput.h"
#include <iostream>

//#define USE_SDL

int main(void){
#ifdef USE_SDL
    DemoPresenter* presenter = new DemoPresenter();
    ParticlesViewInterface* view = new particlesViewSdlAdapter(*presenter, WINDOW_SIDE);
    UserInput* userInput = new UserInput();
#else
    ParticlesView* view = new ParticlesView(WINDOW_SIDE);
    IUserInput* userInput = view;
#endif
    Particles Ps(*view, *userInput);
    Ps.Init();
    do
    {
        Ps.Update();
    }
    while (! userInput->IsQuitRequested());
    delete view;
#ifdef USE_SDL
    delete presenter;
    delete userInput;
#endif
    return 0;

}

