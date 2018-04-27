#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "SDL/include/SDL.h"

#include "j1Video.h"

j1Video::j1Video()
{
	name.create("video");
}

j1Video::~j1Video()
{
}

bool j1Video::Awake(pugi::xml_node &)
{
	return true;
}

bool j1Video::CleanUp()
{
	CloseAVI();
	return true;
}

void j1Video::Initialize(char* path)
{
	// Start Of User Initialization
	angle = 0.0f;                           // Set Starting Angle To Zero
	hdd = DrawDibOpen();                        // Grab A Device Context For Our Dib
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);              // Black Background
	glClearDepth(1.0f);                        // Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);                    // Enable Depth Testing
	glShadeModel(GL_SMOOTH);                   // Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);     // Set Perspective Calculations To Most Accurate

	quadratic = gluNewQuadric();                  // Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);           // Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);              // Create Texture Coords

	glEnable(GL_TEXTURE_2D);                    // Enable Texture Mapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);// Set Texture Max Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// Set Texture Min Filter

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);        // Set The Texture Generation Mode For S To Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);        // Set The Texture Generation Mode For T To Sphere Mapping

	OpenAVI(path);                  // Open The AVI File

									// Create The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void j1Video::OpenAVI(LPCSTR path)
{
	TCHAR   title[100];                     // Will Hold The Modified Window Title

	AVIFileInit();                          // Opens The AVIFile Library

	if (AVIStreamOpenFromFile(&pavi, path, streamtypeVIDEO, 0, OF_READ, NULL) != 0) // Opens The AVI Stream
		LOG("Failed To Open The AVI Stream");

	AVIStreamInfo(pavi, &psi, sizeof(psi));					// Reads Information About The Stream Into psi
	width = psi.rcFrame.right - psi.rcFrame.left;           // Width Is Right Side Of Frame Minus Left
	height = psi.rcFrame.bottom - psi.rcFrame.top;          // Height Is Bottom Of Frame Minus Top

	lastframe = AVIStreamLength(pavi);						// The Last Frame Of The Stream

	mpf = AVIStreamSampleToTime(pavi, lastframe) / lastframe;        // Calculate Rough Milliseconds Per Frame

	bmih.biSize = sizeof(BITMAPINFOHEADER);        // Size Of The BitmapInfoHeader
	bmih.biPlanes = 1;							   // Bitplanes
	bmih.biBitCount = 24;						   // Bits Format We Want (24 Bit, 3 Bytes)
	bmih.biWidth = 256;							   // Width We Want (256 Pixels)
	bmih.biHeight = 256;                           // Height We Want (256 Pixels)
	bmih.biCompression = BI_RGB;                   // Requested Mode = RGB

	hBitmap = CreateDIBSection(hdc, (BITMAPINFO*)(&bmih), DIB_RGB_COLORS, (void**)(&data), NULL, NULL);
	SelectObject(hdc, hBitmap);                           // Select hBitmap Into Our Device Context (hdc)

	pgf = AVIStreamGetFrameOpen(pavi, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);              // Create The PGETFRAME Using Our Request Mode
	if (pgf == NULL)
		LOG("Failed To Open The AVI Frame");

	// Information For The Title Bar (Width / Height / Last Frame)
	wsprintf(title, "AVI Player: Width: %d, Height: %d, Frames: %d", width, height, lastframe);
	App->win->SetTitle(title);
}

bool j1Video::GrabAVIFrame()
{
	LPBITMAPINFOHEADER lpbi;						            // Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame);   // Grab Data From The AVI Stream
	pdata = (char *)lpbi + lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);    // Pointer To Data Returned By AVIStreamGetFrame
																				// (Skip The Header Info To Get To The Data)
																				// Convert Data To Requested Bitmap Format
	DrawDibDraw(hdd, hdc, 0, 0, 256, 256, lpbi, pdata, 0, 0, width, height, 0);
	flip(data);                           // Swap The Red And Blue Bytes (GL Compatability)


	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, data); // Update The Texture
	return false;
}

void j1Video::CloseAVI()
{
	DeleteObject(hBitmap);                      // Delete The Device Dependant Bitmap Object
	DrawDibClose(hdd);                      // Closes The DrawDib Device Context
	AVIStreamGetFrameClose(pgf);                    // Deallocates The GetFrame Resources
	AVIStreamRelease(pavi);                     // Release The Stream
	AVIFileExit();                          // Release The File
}

void j1Video::flip(void * buffer)
{
	void* b = buffer;                       // Pointer To The Buffer
	__asm                               // Assembler Code To Follow
	{
		mov ecx, 256 * 256                    // Set Up A Counter (Dimensions Of Memory Block)
		mov ebx, b                      // Points ebx To Our Data (b)
		label :                          // Label Used For Looping
		mov al, [ebx + 0]                  // Loads Value At ebx Into al
			mov ah, [ebx + 2]                  // Loads Value At ebx+2 Into ah
			mov[ebx + 2], al                  // Stores Value In al At ebx+2
			mov[ebx + 0], ah                  // Stores Value In ah At ebx

			add ebx, 3                   // Moves Through The Data By 3 Bytes
			dec ecx                     // Decreases Our Loop Counter
			jnz label                   // If Not Zero Jump Back To Label
	}
}