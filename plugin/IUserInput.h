#ifndef I_USERINPUT_H
#define I_USERINPUT_H

class IUserInput
{
public:
    IUserInput() = default;
    ~IUserInput() = default;
    virtual bool IsQuitRequested() = 0;
    virtual char CheckKeyPress() = 0;
};

#endif // I_USERINPUT_H
