#ifndef USERINPUT_H
#define USERINPUT_H
#include "IUserInput.h"

class UserInput: public IUserInput
{
public:
    UserInput();
    // IUserInput interface
    bool IsQuitRequested();
    char CheckKeyPress();
private:
    bool QuitRequested;
    char LastKeyPressed;
};

#endif // USERINPUT_H
