#ifndef USERINPUT_H
#define USERINPUT_H
#include "IUserInput.h"

class UserInput: public IUserInput
{
public:
    UserInput();
    bool IsQuitRequested();
    char CheckKeyPress();
    MouseClick CheckMouseClick();
private:
    bool QuitRequested;
    char LastKeyPressed;
    MouseClick LastMouseClick;
    MouseClick LastMouseDown;
};

#endif // USERINPUT_H
