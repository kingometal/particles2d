#include "ParticlesView.h"

ParticlesView::ParticlesView(int side)
    : ParticlesViewInterface()
    , XWindow()
    , IUserInput()
    , Side(side)
{
    XWindow::Open(Side,Side,"Particles");
    XWindow::Clear();
}

ParticlesView::~ParticlesView()
{

}

void ParticlesView::SetColor(EColor color)
{
   switch (color)
   {
   case EColor::Black:
       this->Black();
       break;
   case EColor::White:
       this->White();
       break;
   case EColor::Red:
       this->Red();
       break;
   case EColor::Blue:
       this->Blue();
       break;
   }
}

void ParticlesView::DrawParticle(int /*index*/, EColor color, int x, int y)
{
	DrawParticle(color, x, y);
}

void ParticlesView::DrawParticle(EColor color, int x, int y)
{
    SetColor (color);

    DrawPoint (x-2,y-1);
    DrawPoint (x-2, y);
    DrawPoint(x-2,y+1);

    DrawPoint (x-1,y-2);
    DrawPoint (x-1,y-1);
    DrawPoint (x-1, y);
    DrawPoint(x-1,y+1);
    DrawPoint (x-1,y+2);

    DrawPoint (x,y-2);
    DrawPoint (x,y-1);
    DrawPoint (x, y);
    DrawPoint(x,y+1);
    DrawPoint (x,y+2);

    DrawPoint (x+1,y-2);
    DrawPoint (x+1,y-1);
    DrawPoint (x+1, y);
    DrawPoint(x+1,y+1);
    DrawPoint (x+1,y+2);

    DrawPoint (x+2,y-1);
    DrawPoint (x+2, y);
    DrawPoint(x+2,y+1);
}

char ParticlesView::CheckKeyPress()
{
    return XWindow::CheckKeyPress();
}

bool ParticlesView::IsQuitRequested()
{
    return ('q' == CheckKeyPress());
}

int ParticlesView::GetSide()
{
    return Side;
}

void ParticlesView::DrawScreen()
{
    // nothing to do
}

MouseClick ParticlesView::CheckMouseClick()
{
    MouseClick result;
    result.click = false;
    return result;
}

void ParticlesView::ClearWindow()
{

}

