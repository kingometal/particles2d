#ifndef I_USERINPUT_H
#define I_USERINPUT_H

struct MouseClick
{
    int x;
    int y;
    int dx;
    int dy;
    bool click = false;
};

class IUserInput
{
public:
    IUserInput() = default;
    ~IUserInput() = default;
    virtual bool IsQuitRequested() = 0;
    virtual char CheckKeyPress() = 0;
    virtual MouseClick CheckMouseClick() = 0;
};

#endif // I_USERINPUT_H
