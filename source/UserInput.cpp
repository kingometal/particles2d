#include "UserInput.h"
#include <SDL2/SDL.h>

namespace
{
void ProcessEvents(bool & QuitRequested, char& LastKeyPressed, MouseClick& lastMouseClick, MouseClick& lastMouseDown, bool& windowSizeChanged)
{

    SDL_Event e;
    // Handle user input
    while( SDL_PollEvent( &e ) != 0)
    {
        switch (e.type)
        {
        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                windowSizeChanged = true;
                break;
            }
            break;
        case SDL_QUIT:
            QuitRequested = true;
            break;
        case SDL_KEYDOWN:
            switch(e.key.keysym.sym)
            {
            case SDLK_KP_PLUS:
                LastKeyPressed = '+';
                break;
            case SDLK_KP_MINUS:
                LastKeyPressed = '-';
                break;
            case SDLK_KP_DIVIDE:
                LastKeyPressed = '/';
                break;
            case SDLK_KP_MULTIPLY:
                LastKeyPressed = '*';
                break;
            case SDLK_KP_0:
                LastKeyPressed = '0';
                break;
            case SDLK_KP_1:
                LastKeyPressed = '1';
                break;
            case SDLK_KP_2:
                LastKeyPressed = '2';
                break;
            case SDLK_KP_3:
                LastKeyPressed = '3';
                break;
            case SDLK_KP_4:
                LastKeyPressed = '4';
                break;
            case SDLK_KP_5:
                LastKeyPressed = '5';
                break;
            case SDLK_KP_6:
                LastKeyPressed = '6';
                break;
            case SDLK_KP_7:
                LastKeyPressed = '7';
                break;
            case SDLK_KP_8:
                LastKeyPressed = '8';
                break;
            case SDLK_KP_9:
                LastKeyPressed = '9';
                break;
            case SDLK_q:
                LastKeyPressed = 'q';
                QuitRequested = true;
                break;
            case SDLK_r:
                LastKeyPressed = 'r';
                break;
            case SDLK_a:
                LastKeyPressed = 'a';
                break;
            case SDLK_i:
                LastKeyPressed = 'i';
                break;
            case SDLK_v:
                LastKeyPressed = 'v';
                break;
            case SDLK_e:
                LastKeyPressed = 'e';
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN :
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                lastMouseDown.leftclick = true;
            }
            else if (e.button.button == SDL_BUTTON_RIGHT)
            {
                lastMouseClick.rightclick = true;
            }
            lastMouseDown.x = e.button.x;
            lastMouseDown.y = e.button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                lastMouseClick.leftclick = true;
            }
            else if (e.button.button == SDL_BUTTON_RIGHT)
            {
                lastMouseClick.rightclick = true;
            }
            lastMouseClick.x = ((SDL_MouseButtonEvent&) e).x;
            lastMouseClick.y = ((SDL_MouseButtonEvent&) e).y;
            lastMouseClick.dx = lastMouseClick.x - lastMouseDown.x;
            lastMouseClick.dy = lastMouseClick.y - lastMouseDown.y;
            break;
        }
    }
}
}

UserInput::UserInput()
    : QuitRequested(false)
    , LastKeyPressed(0)
    , LastMouseClick()
    , LastMouseDown()
    , WindowSizeChanged(false)
{
}

bool UserInput::IsQuitRequested()
{
    ProcessEvents(QuitRequested, LastKeyPressed, LastMouseClick, LastMouseDown, WindowSizeChanged);
    return QuitRequested;
}

char UserInput::CheckKeyPress()
{
    char returnValue = LastKeyPressed;
    LastKeyPressed = 0;
    return returnValue;
}

MouseClick UserInput::CheckMouseClick()
{
    MouseClick returnValue = LastMouseClick;
    LastMouseClick.leftclick = false;
    LastMouseClick.rightclick = false;
    return returnValue;
}

bool UserInput::CheckSizeChanged()
{
    bool returnValue = WindowSizeChanged;
    WindowSizeChanged = false;
    return returnValue;
}
