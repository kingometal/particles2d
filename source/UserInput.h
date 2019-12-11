#ifndef USERINPUT_H
#define USERINPUT_H
#include "interfaces/IUserInput.h"

class UserInput: public IUserInput
{
public:
    UserInput();
    bool IsQuitRequested();
    char CheckKeyPress();
    MouseClick CheckMouseClick();
    bool CheckSizeChanged();
private:
    bool QuitRequested;
    char LastKeyPressed;
    MouseClick LastMouseClick;
    MouseClick LastMouseDown;
    bool WindowSizeChanged;
};

#endif // USERINPUT_H
