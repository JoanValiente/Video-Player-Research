#ifndef __j1VIDEO_H__
#define __j1VIDEO_H__

#include "j1Module.h"
#include <windows.h>
#include <gl\gl.h>                            // Header File For The OpenGL32 Library
#include <gl\glu.h>                           // Header File For The GLu32 Library
#include <Vfw.h>                          // Header File For Video For Windows
#pragma comment( lib, "opengl32.lib" )                  // Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )                 // Search For GLu32.lib While Linking
#pragma comment( lib, "vfw32.lib" )                 // Search For VFW32.lib While Linking


class j1Video : public j1Module
{
public:

	j1Video();

	// Destructor
	virtual ~j1Video();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	void Initialize(char* file_path);
	void OpenAVI(LPCSTR path);
	bool GrabAVIFrame();

	void CloseAVI();

	void flip(void* buffer);
private:
	// User Defined Variables
	float       angle;                          // Used For Rotation
	int     next;                           // Used For Animation
	int     frame = 0;                        // Frame Counter
	int     effect;                         // Current Effect
	bool        env = true;                       // Environment Mapping (Default On)
	bool        bg = true;                        // Background (Default On)

	AVISTREAMINFO       psi;                        // Pointer To A Structure Containing Stream Info
	PAVISTREAM      pavi;                       // Handle To An Open Stream
	PGETFRAME       pgf;                        // Pointer To A GetFrame Object
	BITMAPINFOHEADER    bmih;                       // Header Information For DrawDibDraw Decoding
	long            lastframe;                  // Last Frame Of The Stream
	int         width;                      // Video Width
	int         height;                     // Video Height
	char*        pdata;                     // Pointer To Texture Data
	int         mpf;                        // Will Hold Rough Milliseconds Per Frame

	GLUquadricObj *quadratic;                       // Storage For Our Quadratic Objects

	HDRAWDIB hdd;                               // Handle For Our Dib
	HBITMAP hBitmap;                            // Handle To A Device Dependant Bitmap
	HDC hdc = CreateCompatibleDC(0);                    // Creates A Compatible Device Context
	unsigned char* data = 0;                        // Pointer To Our Resized Image
};

#endif // __j1Video_H__
