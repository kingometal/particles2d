//==================================================================================
//	Class for a graphical interface for X11 Windows
//	Haye Hinrichsen, 1997-2006, free software (GNU)
//==================================================================================

/***********************************************************************************
	Usage: See documentation instructions.odt.
	Needs: package xorg-x11
	SaveImage needs the command xgrabsc
	CreateMovie needs jpeg2yuv and yuvscaler
***********************************************************************************/

#ifndef XWINDOWS_H
#define XWINDOWS_H

#include <X11/Xutil.h>

class XWindow
{
public:
// CONSTRUCTOR
	XWindow (void); 

// WINDOW CONTROL
	void Open (const int xsize=600, const int ysize=400, const char* title = "");
	void Clear (void) { XClearWindow(display, window); };
	int  GetX (void) { return xsize; }
	int  GetY (void) { return ysize; }
	inline bool IsOpen (void) { return (window!=0); }
	int  GetWindowID (void) { return (int)window; }
	void Close(void);

// DRAWING
	void DrawPoint (const int x, const int y);
	void DrawPoint (const int x, const int y, bool visible);
	void DrawLine (const int x1, const int y1, const int x2, const int y2);
	void Flush (void) { XFlush(display); }

// WRITING TEXT
//	void SetFontsize (const int index=1);
	void DrawText (const int x, const int y, const char* c);

// COLORS
	void SetColor (const double c); // continuous color scale 0<c<1
	void SetColor (const int k);
	void SetColor (int r, int g, int b);
	void Red (void) 	{ XSetForeground(display, gc, 0xFF0000L); }
	void Yellow (void) 	{ XSetForeground(display, gc, 0xFFFF00L); }
	void Green (void)	{ XSetForeground(display, gc, 0x00FF00L); }
	void Blue (void)	{ XSetForeground(display, gc, 0x0000FFL); }
	void Violet (void)	{ XSetForeground(display, gc, 0xFF00FFL); }
	void Black (void) 	{ XSetForeground(display, gc, 0x000000L); }
	void Grey (void)	{ XSetForeground(display, gc, 0x7F7F7FL); }
	void LightGrey (void)	{ XSetForeground(display, gc, 0xAFAFAFL); }
	void White (void)	{ XSetForeground(display, gc, 0xFFFFFFL); }

// HANDLE KEYBOARD
	bool KeyIsPressed (void);
	char GetCharacter (void) { KeyPressed=false; return (PressedKey); }
	char WaitForKeypress (void) { XFlush(display); while (! KeyIsPressed());
	                              return GetCharacter(); }
	void Wait (void) { while (! KeyIsPressed()); GetCharacter(); }
	char CheckKeyPress (void) { if (KeyIsPressed()) {KeyPressed=false; return (PressedKey);} else return '0';}

// GENERATE  PICTURES
	bool WriteImage (const char* filename);

// GENERATE  MOVIES
	void ClearMovie (void);
	void AddFrame (void);
	void CreateMovie (const char* filename, const int frames=20, 
	                  const int mpeglayer=1);

protected:
	static const int distance = 40;		// shift distance
        int xcorner, ycorner;		// upper left edge
	Display *display;			// Display handler
	Window window;				// Window handler
	GC gc,gc2;				// device context
	XEvent event;				// event flag
	KeySym key;				// key
	char PressedKey, KeyPressed;		// keypress flag
	XSizeHints hint;			// hint default coordinates
	int screen, xsize, ysize;		// window sizes
	unsigned long foreground, background;	// foreground and background colors
	int n;					// number of movie frames dropped
        const char* tmp;			// path of movie frames
	Font font[5];				// Array of fonts (0..4)
	int actual_font;			// actual font size
};


#endif // XWINDOWS_H
 
