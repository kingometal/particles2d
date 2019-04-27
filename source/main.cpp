#include "Particles.h"
#include "Constants.h"
#include "ParticlesViewSdlAdapter.h"
#include "Presenter.h"
#include "UserInput.h"

int main(void)
{
    Presenter* presenter = new Presenter();
    ParticlesViewInterface* view = new ParticlesViewSdlAdapter(*presenter, WINDOW_SIDE);
    UserInput* userInput = new UserInput();
    Particles Ps(*view, *userInput);
    Ps.Init();
    do
    {
        Ps.Update();
    }
    while (! userInput->IsQuitRequested());
    delete view;
    delete presenter;
    delete userInput;
    return 0;
}

