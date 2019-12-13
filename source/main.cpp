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
    Particles* simulation = new Particles(*view, *userInput, *parameters);

    simulation->Init();
    do
    {
        simulation->Update();
    }
    while (! userInput->IsQuitRequested());

    delete simulation;
    delete userInput;
    delete view;
    delete presenter;
    delete parameters;

    return 0;
}

