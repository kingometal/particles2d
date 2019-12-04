#include "Config.h"
#include "Particles.h"
#include "ParticlesViewSdlAdapter.h"
#include "Presenter.h"
#include "UserInput.h"

int main(void)
{
    Presenter* presenter = new Presenter();
    Config* parameters = new Config();
    ParticlesViewInterface* view = new ParticlesViewSdlAdapter(*presenter, parameters->WindowSideSizeX, parameters->WindowSideSizeY, parameters->MaxFPS);
    UserInput* userInput = new UserInput();

    Particles Ps(*view, *userInput, *parameters);
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

