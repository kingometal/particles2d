#include "UserInput.h"
#include <SDL2/SDL.h>

namespace
{
void ProcessEvents(bool & QuitRequested, char& LastKeyPressed, MouseClick& lastMouseClick, MouseClick& lastMouseDown)
{

    SDL_Event e;
    // Handle user input
    while( SDL_PollEvent( &e ) != 0)
    {
        if ((SDL_QUIT == e.type || (SDL_KEYDOWN == e.type && SDLK_q == e.key.keysym.sym) ))
        {
            QuitRequested = true;
        }
        else if (SDL_KEYDOWN == e.type)
        {

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
                break;
            case SDLK_r:
                LastKeyPressed = 'r';
                break;
            case SDLK_a:
                LastKeyPressed = 'a';
                break;

            }
        }
        else if (SDL_MOUSEBUTTONDOWN== e.type)
        {
            lastMouseDown.click = true;
            lastMouseDown.x = ((SDL_MouseButtonEvent&) e).x;
            lastMouseDown.y = ((SDL_MouseButtonEvent&) e).y;
        }
        else if (SDL_MOUSEBUTTONUP == e.type)
        {
            lastMouseClick.click = true;
            lastMouseClick.x = ((SDL_MouseButtonEvent&) e).x;
            lastMouseClick.y = ((SDL_MouseButtonEvent&) e).y;
            lastMouseClick.dx = lastMouseClick.x - lastMouseDown.x;
            lastMouseClick.dy = lastMouseClick.y - lastMouseDown.y;
        }
    }
}
}

UserInput::UserInput()
    : QuitRequested(false)
    , LastKeyPressed(0)
    , LastMouseClick()
    , LastMouseDown()
{
}

bool UserInput::IsQuitRequested()
{
    ProcessEvents(QuitRequested, LastKeyPressed, LastMouseClick, LastMouseDown);
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
    LastMouseClick.click = 0;
    return returnValue;
}
