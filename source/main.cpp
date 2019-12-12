#include "Config.h"
#include "Particles.h"
#include "ParticlesViewSdlAdapter.h"
#include "Presenter.h"
#include "UserInput.h"

int main(void)
{
    Config* parameters = new Config();

    Presenter* presenter = new Presenter();
    if (!presenter->Init(parameters->WindowSideSizeX, parameters->WindowSideSizeY))
    {
        return 0;
    }
    IParticlesView* view = new ParticlesViewSdlAdapter(*presenter);
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

