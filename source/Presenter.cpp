#include "Presenter.h"
#include "RGBData.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

namespace MySDL
{
bool Init(SDL_Window** window, SDL_Renderer **renderer, TTF_Font** font, int width, int height, string title, string fontPath, int fontSize, Uint32 sdlWindowFlags, Uint32 sdlRendererFlags)
{
    bool success = false;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    }
    else
    {
        if (0 > TTF_Init())
        {
            cout << "TTF could not initialize! TTF_Error: " << TTF_GetError() << endl;
        }
        else
        {
            *font = TTF_OpenFont(fontPath.c_str(), fontSize); //this opens a font style and sets a size
            if (NULL == *font)
            {
                cout << "can not create font! TTF_Error: " << TTF_GetError() << endl;
                TTF_Quit();
                SDL_Quit();
            }
            else
            {
                *window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, sdlWindowFlags);
                if( window == NULL )
                {
                    cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
                    TTF_CloseFont(*font);
                    TTF_Quit();
                    SDL_Quit();
                }
                else
                {
                    *renderer = SDL_CreateRenderer(*window, -1, sdlRendererFlags);
                    if (NULL == renderer)
                    {
                        cout << "Can not create renderer! SDL_Error: " << SDL_GetError() << endl;
                        SDL_DestroyWindow(*window);
                        TTF_CloseFont(*font);
                        TTF_Quit();
                        SDL_Quit();
                    }
                    else
                    {
                        success = true;
                    }
                }
            }
        }
    }
    return success;
}

void DeInit(SDL_Window** window, SDL_Renderer **renderer, TTF_Font** font)
{
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow( *window );
    TTF_CloseFont(*font);
    TTF_Quit();
    SDL_Quit();
}
}


class Presenter::PresenterImpl
{
public:
    PresenterImpl()
    {
    }

    ~PresenterImpl()
    {
        MySDL::DeInit(&gWindow, &Renderer, &font);
    }

    void PrintRendererInfo(SDL_Renderer *renderer)
    {
        int numdrivers = SDL_GetNumRenderDrivers ();
        cout << "Render driver count: " << numdrivers << endl;
        for (int i=0; i<numdrivers; i++) {
            SDL_RendererInfo drinfo;
            SDL_GetRenderDriverInfo (i, &drinfo);
            cout << "Driver name ("<<i<<"): " << drinfo.name << endl;
            if (drinfo.flags & SDL_RENDERER_SOFTWARE) cout << " the renderer is  a software fallback" << endl;
            if (drinfo.flags & SDL_RENDERER_ACCELERATED) cout << " the renderer  uses hardware acceleration" << endl;
            if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC) cout << " present is synchronized with the refresh rate" << endl;
            if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE) cout << " the  renderer supports rendering to texture" << endl;
        }

        SDL_RendererInfo drinfo;
        SDL_GetRendererInfo (renderer, &drinfo);
        cout << "Current Driver name " << drinfo.name << endl;
        if (drinfo.flags & SDL_RENDERER_SOFTWARE) cout << " the renderer is  a software fallback" << endl;
        if (drinfo.flags & SDL_RENDERER_ACCELERATED) cout << " the renderer  uses hardware acceleration" << endl;
        if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC) cout << " present is synchronized with the refresh rate" << endl;
        if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE) cout << " the  renderer supports rendering to texture" << endl;
    }

    void PrintText(const char* text, SDL_Color& color, int x, int y)
    {
        SDL_Surface* surfaceMessage = TTF_RenderText(font, text, color, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
        SDL_SetColorKey(surfaceMessage, SDL_TRUE, 0);
        if (surfaceMessage)
        {
            SDL_Texture* Message = SDL_CreateTextureFromSurface(Renderer, surfaceMessage); //now you can convert it into a texture
            if (Message)
            {
                SDL_Rect Message_rect; //create a rect
                Message_rect.x = x;  //controls the rect's x coordinate
                Message_rect.y = y; // controls the rect's y coordinte
                Message_rect.w = surfaceMessage->w; // controls the width of the rect
                Message_rect.h = surfaceMessage->h; // controls the height of the rect

                SDL_RenderCopy(Renderer, Message, NULL, &Message_rect);
                SDL_DestroyTexture(Message);
            }
            SDL_FreeSurface(surfaceMessage);
        }
    }

    SDL_Window* gWindow = NULL;
    SDL_Renderer* Renderer = NULL;
    TTF_Font* font = NULL;
    double lastFPSOutputTime;
};

Presenter::Presenter()
    : Pimpl(new PresenterImpl())
{
}

Presenter::~Presenter()
{
    delete Pimpl;
}

void Presenter::StorePoint(int x, int y, const RGBData &data)
{
    SDL_SetRenderDrawColor(Pimpl->Renderer, data.GetR(), data.GetG(), data.GetB(), data.GetA());
    SDL_RenderDrawPoint(Pimpl->Renderer, x, y);
}

void Presenter::StoreLine(int x, int y, int dx, int dy, const RGBData &data)
{
    SDL_SetRenderDrawColor(Pimpl->Renderer, data.GetR(), data.GetG(), data.GetB(), data.GetA());
    SDL_RenderDrawLine(Pimpl->Renderer, x, y, x+dx, y+dy);
}

SDL_Color RGBDataToSDLColor(const RGBData &color)
{
    return SDL_Color {color.GetR(), color.GetG(), color.GetB(), color.GetA()};
}

void Presenter::StoreText(const char *text, const RGBData &color, int x, int y)
{
    SDL_Color fontColor = RGBDataToSDLColor(color);  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    Pimpl->PrintText(text, fontColor, x, y);
}


void Presenter::StoreRectangle(int x, int y, int dx, int dy, const RGBData &data)
{
    SDL_SetRenderDrawColor(Pimpl->Renderer, data.GetR(), data.GetG(), data.GetB(), data.GetA());
    SDL_Rect rect = {x, y, dx, dy};
    SDL_RenderFillRect(Pimpl->Renderer, &rect);
}

void Presenter::Present()
{
    SDL_RenderPresent(Pimpl->Renderer);

}

bool Presenter::Init(int width, int height)
{
    return MySDL::Init(&Pimpl->gWindow, &Pimpl->Renderer, &Pimpl->font, width, height, "Particles2D", "FreeMono.ttf", 24, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, SDL_RENDERER_SOFTWARE);
}

void Presenter::ClearWindow(RGBData &color)
{
    SDL_SetRenderDrawColor(Pimpl->Renderer, color.GetR(), color.GetG(), color.GetB(), color.GetA());
    SDL_RenderClear(Pimpl->Renderer);
}

void Presenter::GetWindowSize(int &x, int &y) const
{
    return SDL_GL_GetDrawableSize(Pimpl->gWindow, &x, &y);
}
