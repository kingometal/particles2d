#include "Presenter.h"
#include "RGBData.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

class Presenter::PresenterImpl
{
public:
    PresenterImpl(int height, int width, bool resizable)
    {
        Init(&gWindow, &renderer, &font, width, height, resizable);
        PrintRendererInfo(renderer);
    }

    ~PresenterImpl()
    {
        DeInit(&gWindow, &renderer, &font);
    }

    void Draw(int x, int y, const RGBData& rgbData)
    {
        SDL_SetRenderDrawColor(renderer, rgbData.GetR(), rgbData.GetG(), rgbData.GetB(), rgbData.GetA());
        SDL_RenderDrawPoint(renderer, x, y);
    }

    void DrawLine(int x, int y, int dx, int dy, const RGBData& rgbData)
    {
        SDL_SetRenderDrawColor(renderer, rgbData.GetR(), rgbData.GetG(), rgbData.GetB(), rgbData.GetA());
        SDL_RenderDrawLine(renderer, x, y, x+dx, y+dy);
    }

    void DrawRectangle(int x, int y, int dx, int dy, const RGBData& rgbData)
    {
        SDL_SetRenderDrawColor(renderer, rgbData.GetR(), rgbData.GetG(), rgbData.GetB(), rgbData.GetA());
        SDL_Rect rect = {x, y, dx, dy};
        SDL_RenderFillRect(renderer, &rect);
    }

    void Present(int maxFps)
    {
        SDL_Color fontColor = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
        double secondsCounter = SDL_GetPerformanceCounter()/(double) SDL_GetPerformanceFrequency();
        double fps = 1.0/(secondsCounter - lastFPSOutputTime);

        if (0 == maxFps || fps < maxFps)
        {
            PrintText(renderer, std::to_string(fps).c_str(), fontColor, font, 0,0 );
            SDL_RenderPresent(renderer);
            lastFPSOutputTime = secondsCounter;
        }
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

    bool Init(SDL_Window** window, SDL_Renderer **renderer, TTF_Font** font, int width, int height, bool resizable)
    {
        bool success = false;
        if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            if (0 > TTF_Init())
            {
                printf( "TTF could not initialize! TTF_Error: %s\n", TTF_GetError() );
            }
            else
            {
                *font = TTF_OpenFont("FreeMono.ttf", 24); //this opens a font style and sets a size
                if (NULL == *font)
                {
                    std::cout << "can not create font" << std::endl;
                    std::cout << TTF_GetError() << std::endl;
                }
                else
                {
                    *window = SDL_CreateWindow( "Simple lightweight per-pixel drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | (resizable?SDL_WINDOW_RESIZABLE:0));
                    if( window == NULL )
                    {
                        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
                    }
                    else
                    {
                        *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);
                        //*renderer = SDL_CreateRenderer(*window, -1, 0);
                        if (NULL == renderer)
                        {
                            printf( "Can not get surface! SDL_Error: %s\n", SDL_GetError() );
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
        TTF_CloseFont(*font);
        TTF_Quit();
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow( *window );
        SDL_Quit();
    }

    void PrintText(SDL_Renderer* renderer, const char* text, SDL_Color& color, TTF_Font* font, int x, int y)
    {
        SDL_Color backgroundColor {0,0,0,0};
        SDL_Surface* surfaceMessage = TTF_RenderText(font, text, color, backgroundColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
        if (surfaceMessage)
        {
            SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
            if (Message)
            {
                SDL_Rect Message_rect; //create a rect
                Message_rect.x = x;  //controls the rect's x coordinate
                Message_rect.y = y; // controls the rect's y coordinte
                Message_rect.w = surfaceMessage->w; // controls the width of the rect
                Message_rect.h = surfaceMessage->h; // controls the height of the rect

                SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
                SDL_DestroyTexture(Message);
            }
            SDL_FreeSurface(surfaceMessage);
        }
    }

private:
    SDL_Window* gWindow = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
    double lastFPSOutputTime;
};

Presenter::Presenter()
    : Pimpl(NULL)
{
}

Presenter::~Presenter()
{
    delete Pimpl;
}

void Presenter::StorePoint(int x, int y, const RGBData &data)
{
    Pimpl->Draw(x, y, data);
}

void Presenter::StoreLine(int x, int y, int dx, int dy, const RGBData &data)
{
    Pimpl->DrawLine(x, y, dx, dy, data);
}

void Presenter::StoreRectangle(int x, int y, int dx, int dy, const RGBData &data)
{
    Pimpl->DrawRectangle(x, y, dx, dy, data);
}

void Presenter::Present(int maxFps)
{
    Pimpl->Present(maxFps);
}

void Presenter::Init(int height, int width, bool resizable)
{
    Pimpl = new PresenterImpl(height, width, resizable);
}

void Presenter::ClearWindow(RGBData &color)
{
}
