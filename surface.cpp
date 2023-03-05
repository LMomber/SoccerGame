// Template, BUAS version https://www.buas.nl/games
// IGAD/BUAS(NHTV)/UU - Jacco Bikker - 2006-2020

#include "surface.h"
#include "template.h"
#include <cassert>
#include <cstring>
#include "FreeImage.h"
#include <iostream>

using namespace std;

namespace Tmpl8 {

	void NotifyUser(const char* s);
	char Surface::s_Font[51][5][6];	//Set 3-dimensional array with dimensions 51x5x6
	bool Surface::fontInitialized = false;

	// -----------------------------------------------------------
	// True-color surface class implementation
	// -----------------------------------------------------------

	//Surface constructor
	Surface::Surface(int a_Width, int a_Height, Pixel* a_Buffer, int a_Pitch) :
		m_Buffer(a_Buffer),
		m_Width(a_Width),
		m_Height(a_Height),
		m_Pitch(a_Pitch)
	{
	}

	//Surface constructor
	Surface::Surface(int a_Width, int a_Height) :
		m_Width(a_Width),
		m_Height(a_Height),
		m_Pitch(a_Width),
		m_Flags(OWNER)
	{
		m_Buffer = static_cast<Pixel*>(MALLOC64((unsigned int)a_Width * (unsigned int)a_Height * sizeof(Pixel)));
	}

	//Surface constructor from file (image)
	Surface::Surface(const char* a_File)
	{
		FILE* f = fopen(a_File, "rb");
		if (!f) //If file is not available, send error
		{
			char t[128];
			sprintf(t, "File not found: %s", a_File);
			NotifyUser(t);
			return;
		}
		else fclose(f);
		LoadImage(a_File);
	}

	//Load image
	void Surface::LoadImage(const char* a_File)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		// Sets the format of "fif" to NULL.
		fif = FreeImage_GetFileType(a_File, 0);
		// Check the file signature of "a_File" and deduce its format.
		if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(a_File);
		// If format is not found with "FreeImage_GetFileType()", look for other formats in the file extension. (CUT, PCD, TARGA and WBMP)
		FIBITMAP* tmp = FreeImage_Load(fif, a_File);
		// Decode bitmap "a_File" for format "fif" and store it as a FIBITMAP.
		FIBITMAP* dib = FreeImage_ConvertTo32Bits(tmp);
		// Convert bitmap "tmp" to 32 bits.
		FreeImage_Unload(tmp);
		// Delete FIBITMAP tmp.
		m_Width = m_Pitch = FreeImage_GetWidth(dib);
		// Return width of bitmap in pixel units.
		m_Height = FreeImage_GetHeight(dib);
		// Return heigth of bitmap in pixel units.
		m_Buffer = (Pixel*)MALLOC64(m_Width * m_Height * sizeof(Pixel));
		// Allocates dynamic memory for the amount of pixels making up the surface.
		if (m_Buffer) // If "m_Buffer" isn't a null pointer:
		{
			m_Flags = OWNER;
			assert(m_Pitch != 0); // Stop program if the pitch = 0.
			for (int y = 0; y < m_Height; y++)
			{
				if (m_Pitch != 0)
				{
					unsigned char* line = FreeImage_GetScanLine(dib, m_Height - 1 - y);
					// Get the pointer to the start of the scanline, starting at the bottom.
					memcpy(m_Buffer + (y * m_Pitch), line, m_Width * sizeof(Pixel));
					// Copy the values, pointed to by the scanline "line", to the corresponding memory block.
				}
			}
		}
		FreeImage_Unload(dib);
		// Delete FIBITMAP dib.
	}

	//Surface destructor
	Surface::~Surface()
	{
		if (m_Flags & OWNER)
		{
			FREE64(m_Buffer);
		}
	}

	//Make the surface color "a_Color"
	void Surface::Clear(Pixel a_Color)
	{
		int s = m_Width * m_Height; // The amount of pixels on the screen.
		for (int i = 0; i < s; i++) m_Buffer[i] = a_Color; // Color each pixel on the surface color "a_Color".
	}

	//Print text in the middle of the surface
	void Surface::Centre(const char* a_String, int y1, Pixel color)
	{
		cout << m_Width << "\n";
		int x = (m_Width - (int)strlen(a_String) * 6) / 2;
		// Calculate the begin position for the centre text. Each letter is 5 pixels long + a blank pixel.
		Print(a_String, x, y1, color);
	}

	void Surface::Print(const char* a_String, int x1, int y1, Pixel color, int width)
	{
		if (!fontInitialized)
		{
			InitCharset();
			fontInitialized = true;
		}
		Pixel* t = m_Buffer + x1 + y1 * m_Pitch;
		for (int i = 0; i < (int)(strlen(a_String)); i++, t += 6 * width)
		{
			long pos = 0;
			if ((a_String[i] >= 'A') && (a_String[i] <= 'Z')) pos = s_Transl[(unsigned short)(a_String[i] - ('A' - 'a'))];
			else pos = s_Transl[(unsigned short)a_String[i]];
			Pixel* a = t;
			char* c = (char*)s_Font[pos];
			for (int v = 0; v < 5; v++, c++, a += width * m_Pitch) {
				for (int h = 0; h < 5 * width; h += width) {
					if (*c++ == 'o') {
						for (int w = 0; w < width; w++)
							for (int j = 0; j < width; j++)
								a[w + h + j * m_Pitch] = color, a[w + h + (j + 1) * m_Pitch] = 0;
					}
				}
			}
		}
	}

	//Resize surface to screensize: src.Resize(dst); using bilinear interpolation
	void Surface::Resize(Surface* a_Orig)
	{
		Pixel* src = a_Orig->GetBuffer(), * dst = m_Buffer;
		// *src = the memory location of the original surface.
		// *dst = the memory location for this surface.
		int u, v, owidth = a_Orig->GetWidth(), oheight = a_Orig->GetHeight();
		int dx = (owidth << 10) / m_Width, dy = (oheight << 10) / m_Height;
		// dx & dy are the scaling factors. Bitshift is used to avoid floating point arithmetics and increase performance.
		for (v = 0; v < m_Height; v++) // For every y on the surface:
		{
			for (u = 0; u < m_Width; u++) // For every x on the surface:
			{
				int su = u * dx, sv = v * dy;
				// new x and y value multiplied by 1023.
				Pixel* s = src + (su >> 10) + (sv >> 10) * owidth;
				// s is the new memory location for the corresponding x and y.
				int ufrac = su & 1023, vfrac = sv & 1023;
				// Get the fractional parts of su and sv (since su & sv are multiplied by 1023, every bit until 1023 is behind the decimal).
				int w4 = (ufrac * vfrac) >> 12; // Calculate weights and divide by 4 (from the original number, >> 10)
				int w3 = ((1023 - ufrac) * vfrac) >> 12;
				int w2 = (ufrac * (1023 - vfrac)) >> 12;
				int w1 = ((1023 - ufrac) * (1023 - vfrac)) >> 12;
				int x2 = ((su + dx) > ((owidth - 1) << 10)) ? 0 : 1; // If the new width is bigger than the old width, x2 = 0, else x2 = 1.
				int y2 = ((sv + dy) > ((oheight - 1) << 10)) ? 0 : 1;// If the new height is bigger than the old height, y2 = 0, else y2 = 1.
				Pixel p1 = *s, p2 = *(s + x2), p3 = *(s + owidth * y2), p4 = *(s + owidth * y2 + x2);
				// p1 is the value in memory location s.
				// p2 is the value in memory location (s + x2), which is one to the right of s, or s.
				// p3 is the value in memory location (s + (owidth * y2)), which is one lower than s, or s.
				// p4 is the value in memory location (s + (owidth * y2) + x2), which is one to the right and one lower than s, or s.
				unsigned int r = (((p1 & RedMask) * w1 + (p2 & RedMask) * w2 + (p3 & RedMask) * w3 + (p4 & RedMask) * w4) >> 8) & RedMask;
				unsigned int g = (((p1 & GreenMask) * w1 + (p2 & GreenMask) * w2 + (p3 & GreenMask) * w3 + (p4 & GreenMask) * w4) >> 8) & GreenMask;
				unsigned int b = (((p1 & BlueMask) * w1 + (p2 & BlueMask) * w2 + (p3 & BlueMask) * w3 + (p4 & BlueMask) * w4) >> 8) & BlueMask;
				*(dst + u + v * m_Pitch) = (Pixel)(r + g + b);
				// Give the new memory location the right color value.
			}
		}
	}

	//Outcode for line clipping
	int LineOutCode(float x, float y, float xMin, float xMax, float yMin, float yMax)
	{
		return (((x) < xMin) ? 1 : (((x) > xMax) ? 2 : 0)) + (((y) < yMin) ? 4 : (((y) > yMax) ? 8 : 0));
		/*
		*	if (x < xMin)
		*	{
		*		return 1; // LEFT
		*	}
		*	else if (x > xMax)
		*	{
		*		return 2; // RIGHT
		*	}
		*	else
		*	{
		*		return 0; // CENTRE
		*	}
		*	+++++++++++++++++++++++
		*	if (y < yMin)
		*	{
		*		return 4; // BOTTOM
		*	}
		*	else if (y > yMax)
		*	{
		*		return 8; // TOP
		*	}
		*	else
		*	{
		*		return 0; // CENTRE
		*	}
		*   return x + y;
		*/
	}

	//Checks and deals with line clipping, then draws a line
	void Surface::Line(float x1, float y1, float x2, float y2, Pixel c)
	{
		// clip (Cohen-Sutherland, https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm)
		const float xmin = 0, ymin = 0, xmax = ScreenWidth - 1, ymax = ScreenHeight - 1;
		int c0 = LineOutCode(x1, y1, xmin, xmax, ymin, ymax), c1 = LineOutCode(x2, y2, xmin, xmax, ymin, ymax);
		bool accept = false;
		while (1)
		{
			if (!(c0 | c1)) { accept = true; break; } // if c0 and c1 are both 0, and thus trivial accepts: accept = true and go further to line 243.
			else if (c0 & c1) break; else // if c0 and c1 are both > 0, trivial rejection. Break while loop and end Line function. Else:
			{
				float x = 1.0f, y = 1.0f;
				// Calculations to find which point is out of border:
				const int co = c0 ? c0 : c1; // If c0 > 0, co = c0, else co = c1
				// If (x2,y2) is out of border:
				if (co & 8) x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1), y = ymax;			    // TOP
				else if (co & 4) x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1), y = ymin;	    // BOTTOM
				else if (co & 2) y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1), x = xmax;	    // RIGHT
				else if (co & 1) y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1), x = xmin;	    // LEFT

				if (co == c0) x1 = x, y1 = y, c0 = LineOutCode(x1, y1, xmin, xmax, ymin, ymax);// If (x1,y1) is out of border: make point 1 (0,0) and make new outcode for c0.
				else x2 = x, y2 = y, c1 = LineOutCode(x2, y2, xmin, xmax, ymin, ymax);			// Else: make point 2 (0,0) and make new outcode for c1.
			}
		}
		if (!accept) return;                    // If one of the points was out of border, run Line function from the start.
		float b = x2 - x1;						// Width
		float h = y2 - y1;						// Height
		float l = fabsf(b);					// l
		if (fabsf(h) > l) l = fabsf(h);	// If the heigt of the line > width of the line, l = absolute value of height.
		int il = (int)l;
		float dx = b / (float)l;
		float dy = h / (float)l;
		for (int i = 0; i <= il; i++) // For the widht or the height, depending on which is bigger:
		{
			*(m_Buffer + (int)x1 + (int)y1 * m_Pitch) = c; // Draw pixel with set color.
			x1 += dx, y1 += dy; // Go to next pixel value of the line.
		}
	}

	//Draws 1 pixel
	void Surface::Plot(int x, int y, Pixel c)
	{
		// If inside the boundaries of the screen, change the color of the adress inside the m_buffer array
		if ((x >= 0) && (y >= 0) && (x < m_Width) && (y < m_Height)) m_Buffer[x + y * m_Pitch] = c;
	}

	//Draws a box
	void Surface::Box(int x1, int y1, int x2, int y2, Pixel c)
	{
		// Draw 4 connecting lines.
		Line((float)x1, (float)y1, (float)x2, (float)y1, c);
		Line((float)x2, (float)y1, (float)x2, (float)y2, c);
		Line((float)x1, (float)y2, (float)x2, (float)y2, c);
		Line((float)x1, (float)y1, (float)x1, (float)y2, c);
	}

	//Draws a bar of pixels on the surface from x1 and y1 to x2 and y2
	void Surface::Bar(int x1, int y1, int x2, int y2, Pixel c)
	{
		Pixel* a = x1 + y1 * m_Pitch + m_Buffer;
		for (int y = y1; y <= y2; y++)
		{
			assert(y <= m_Height);
			for (int x = 0; x <= (x2 - x1); x++) a[x] = c;
			a += m_Pitch;
		}
	}

	//Copies the content of the source surface to the destination surface
	void Surface::CopyTo(Surface* a_Dst, int a_X, int a_Y)
	{
		Pixel* dst = a_Dst->GetBuffer();
		// Memory location of destination.
		Pixel* src = m_Buffer;
		// Memory location of source.
		if ((src) && (dst)) // If src and dst both have valid memory locations:
		{
			int srcwidth = m_Width;
			int srcheight = m_Height;
			int srcpitch = m_Pitch;
			int dstwidth = a_Dst->GetWidth();
			int dstheight = a_Dst->GetHeight();
			int dstpitch = a_Dst->GetPitch();
			if ((srcwidth + a_X) > dstwidth) srcwidth = dstwidth - a_X;
			// If the srcwidth starting on x does not fit in the dstwidth, srcwidth = dstwidth - x.
			if ((srcheight + a_Y) > dstheight) srcheight = dstheight - a_Y;
			// If the srcheight starting on y does not fit in the dstheight, srcheight = dstheight - y.
			if (a_X < 0) src -= a_X, srcwidth += a_X, a_X = 0;
			// If starting x < 0, src -= x (=src + abs(x)), srcwidth += x (= width - x) and x = 0.
			// If x < 0, shape the allocated memory location and the width to the size of x (original - x).
			if (a_Y < 0) src -= a_Y * srcpitch, srcheight += a_Y, a_Y = 0;
			// If starting y < 0, src -= y (=src + abs(y)), srcheight += y (because y is neg: height - y) and y = 0.
			// If y < 0, shape the allocated memory location and the height to the size of y (original - y).
			if ((srcwidth > 0) && (srcheight > 0)) // If the src width and height are still positive after subtracting x and y:
			{
				dst += a_X + dstpitch * a_Y;
				// dst = dst + x and y
				for (int y = 0; y < srcheight; y++) // For every y of the source height:
				{
					memcpy(dst, src, srcwidth * 4);
					// Copy the source values to the destination location.
					// srcwidth is multiplied by 4, because each pixel takes up 4 bytes of memory, AKA the pitch.
					dst += dstpitch;
					// Go to the memory location of the next dst pixel.
					src += srcpitch;
					// Go to the memory location of the next src pixel.
				}
			}
		}
	}

	//Blends the content of the source surface with the contents of the destination surface
	void Surface::BlendCopyTo(Surface* a_Dst, int a_X, int a_Y)
	{
		Pixel* dst = a_Dst->GetBuffer();
		Pixel* src = m_Buffer;
		if ((src) && (dst))
		{
			int srcwidth = m_Width;
			int srcheight = m_Height;
			int srcpitch = m_Pitch;
			int dstwidth = a_Dst->GetWidth();
			int dstheight = a_Dst->GetHeight();
			int dstpitch = a_Dst->GetPitch();
			if ((srcwidth + a_X) > dstwidth) srcwidth = dstwidth - a_X;
			if ((srcheight + a_Y) > dstheight) srcheight = dstheight - a_Y;
			if (a_X < 0) src -= a_X, srcwidth += a_X, a_X = 0;
			if (a_Y < 0) src -= a_Y * srcpitch, srcheight += a_Y, a_Y = 0;
			if ((srcwidth > 0) && (srcheight > 0))
			{
				dst += a_X + dstpitch * a_Y;
				for (int y = 0; y < srcheight; y++)
				{
					// For every x of scrwidth:
					for (int x = 0; x < srcwidth; x++) dst[x] = AddBlend(dst[x], src[x]);
					dst += dstpitch;
					src += srcpitch;
				}
			}
		}
	}

	//Makes a 2D-array to draw characters from pixels
	void Surface::SetChar(int c, char* c1, char* c2, char* c3, char* c4, char* c5)
	{
		strcpy(s_Font[c][0], c1);
		strcpy(s_Font[c][1], c2);
		strcpy(s_Font[c][2], c3);
		strcpy(s_Font[c][3], c4);
		strcpy(s_Font[c][4], c5);
	}

	//Characters "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/" written in pixels. Only uppercase
	void Surface::InitCharset()
	{
		SetChar(0, ":ooo:", "o:::o", "ooooo", "o:::o", "o:::o");
		SetChar(1, "oooo:", "o:::o", "oooo:", "o:::o", "oooo:");
		SetChar(2, ":oooo", "o::::", "o::::", "o::::", ":oooo");
		SetChar(3, "oooo:", "o:::o", "o:::o", "o:::o", "oooo:");
		SetChar(4, "ooooo", "o::::", "oooo:", "o::::", "ooooo");
		SetChar(5, "ooooo", "o::::", "ooo::", "o::::", "o::::");
		SetChar(6, ":oooo", "o::::", "o:ooo", "o:::o", ":ooo:");
		SetChar(7, "o:::o", "o:::o", "ooooo", "o:::o", "o:::o");
		SetChar(8, "::o::", "::o::", "::o::", "::o::", "::o::");
		SetChar(9, ":::o:", ":::o:", ":::o:", ":::o:", "ooo::");
		SetChar(10, "o::o:", "o:o::", "oo:::", "o:o::", "o::o:");
		SetChar(11, "o::::", "o::::", "o::::", "o::::", "ooooo");
		SetChar(12, "oo:o:", "o:o:o", "o:o:o", "o:::o", "o:::o");
		SetChar(13, "o:::o", "oo::o", "o:o:o", "o::oo", "o:::o");
		SetChar(14, ":ooo:", "o:::o", "o:::o", "o:::o", ":ooo:");
		SetChar(15, "oooo:", "o:::o", "oooo:", "o::::", "o::::");
		SetChar(16, ":ooo:", "o:::o", "o:::o", "o::oo", ":oooo");
		SetChar(17, "oooo:", "o:::o", "oooo:", "o:o::", "o::o:");
		SetChar(18, ":oooo", "o::::", ":ooo:", "::::o", "oooo:");
		SetChar(19, "ooooo", "::o::", "::o::", "::o::", "::o::");
		SetChar(20, "o:::o", "o:::o", "o:::o", "o:::o", ":oooo");
		SetChar(21, "o:::o", "o:::o", ":o:o:", ":o:o:", "::o::");
		SetChar(22, "o:::o", "o:::o", "o:o:o", "o:o:o", ":o:o:");
		SetChar(23, "o:::o", ":o:o:", "::o::", ":o:o:", "o:::o");
		SetChar(24, "o:::o", "o:::o", ":oooo", "::::o", ":ooo:");
		SetChar(25, "ooooo", ":::o:", "::o::", ":o:::", "ooooo");
		SetChar(26, ":ooo:", "o::oo", "o:o:o", "oo::o", ":ooo:");
		SetChar(27, "::o::", ":oo::", "::o::", "::o::", ":ooo:");
		SetChar(28, ":ooo:", "o:::o", "::oo:", ":o:::", "ooooo");
		SetChar(29, "oooo:", "::::o", "::oo:", "::::o", "oooo:");
		SetChar(30, "o::::", "o::o:", "ooooo", ":::o:", ":::o:");
		SetChar(31, "ooooo", "o::::", "oooo:", "::::o", "oooo:");
		SetChar(32, ":oooo", "o::::", "oooo:", "o:::o", ":ooo:");
		SetChar(33, "ooooo", "::::o", ":::o:", "::o::", "::o::");
		SetChar(34, ":ooo:", "o:::o", ":ooo:", "o:::o", ":ooo:");
		SetChar(35, ":ooo:", "o:::o", ":oooo", "::::o", ":ooo:");
		SetChar(36, "::o::", "::o::", "::o::", ":::::", "::o::");
		SetChar(37, ":ooo:", "::::o", ":::o:", ":::::", "::o::");
		SetChar(38, ":::::", ":::::", "::o::", ":::::", "::o::");
		SetChar(39, ":::::", ":::::", ":ooo:", ":::::", ":ooo:");
		SetChar(40, ":::::", ":::::", ":::::", ":::o:", "::o::");
		SetChar(41, ":::::", ":::::", ":::::", ":::::", "::o::");
		SetChar(42, ":::::", ":::::", ":ooo:", ":::::", ":::::");
		SetChar(43, ":::o:", "::o::", "::o::", "::o::", ":::o:");
		SetChar(44, "::o::", ":::o:", ":::o:", ":::o:", "::o::");
		SetChar(45, ":::::", ":::::", ":::::", ":::::", ":::::");
		SetChar(46, "ooooo", "ooooo", "ooooo", "ooooo", "ooooo");
		SetChar(47, "::o::", "::o::", ":::::", ":::::", ":::::"); // Tnx Ferry
		SetChar(48, "o:o:o", ":ooo:", "ooooo", ":ooo:", "o:o:o");
		SetChar(49, "::::o", ":::o:", "::o::", ":o:::", "o::::");
		char c[] = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/"; // Array with all characters.
		int i;
		for (i = 0; i < 256; i++) s_Transl[i] = 45; // Makes every value of array "s_Transl" 45 (which is whitespace in the char set).
		for (i = 0; i < 50; i++) s_Transl[(unsigned char)c[i]] = i; // Overwrites the array index belonging to the charset to the right values.
	}

	//Scales a surface's color by factor a_Scale
	void Surface::ScaleColor(unsigned int a_Scale)
	{
		int s = m_Pitch * m_Height;
		for (int i = 0; i < s; i++)
		{
			Pixel c = m_Buffer[i];
			unsigned int rb = (((c & (RedMask | BlueMask)) * a_Scale) >> 5) & (RedMask | BlueMask);
			unsigned int g = (((c & GreenMask) * a_Scale) >> 5) & GreenMask;
			m_Buffer[i] = rb + g;
		}
	}


	//Sprite constructor
	Sprite::Sprite(Surface* a_Surface, unsigned int a_NumFrames) :
		m_Width(static_cast<int>(a_Surface->GetWidth() / a_NumFrames)),
		m_Height(a_Surface->GetHeight()),
		m_Pitch(a_Surface->GetWidth()),
		m_NumFrames(a_NumFrames),
		m_CurrentFrame(0),
		m_Flags(0),
		m_Start(new unsigned int* [a_NumFrames]),
		m_Surface(a_Surface)
	{
		InitializeStartData();
	}

	//Sprite deconstructor
	Sprite::~Sprite()
	{
		delete m_Surface;
		for (unsigned int i = 0; i < m_NumFrames; i++) delete m_Start[i];
		delete[] m_Start;
	}

	//Draws a sprite
	void Sprite::Draw(Surface* a_Target, int a_X, int a_Y)
	{
		if ((a_X < -m_Width) || (a_X > (a_Target->GetWidth() + m_Width))) return;
		if ((a_Y < -m_Height) || (a_Y > (a_Target->GetHeight() + m_Height))) return;
		int x1 = a_X, x2 = a_X + m_Width;
		int y1 = a_Y, y2 = a_Y + m_Height;
		Pixel* src = GetBuffer() + m_CurrentFrame * m_Width;
		if (x1 < 0)
		{
			src += -x1;
			x1 = 0;
		}
		if (x2 > a_Target->GetWidth()) x2 = a_Target->GetWidth();
		if (y1 < 0)
		{
			src += -y1 * m_Pitch;
			y1 = 0;
		}
		if (y2 > a_Target->GetHeight()) y2 = a_Target->GetHeight();
		Pixel* dest = a_Target->GetBuffer();
		int xs;
		const int dpitch = a_Target->GetPitch();
		if ((x2 > x1) && (y2 > y1))
		{
			unsigned int addr = y1 * dpitch + x1;
			const int width = x2 - x1;
			const int height = y2 - y1;
			for (int y = 0; y < height; y++)
			{
				const int line = y + (y1 - a_Y);
				const int lsx = static_cast<int>(m_Start[m_CurrentFrame][line]) + a_X;
				if (m_Flags & FLARE)
				{
					xs = (lsx > x1) ? lsx - x1 : 0;
					for (int x = xs; x < width; x++)
					{
						const Pixel c1 = *(src + x);
						if (c1 & 0xffffff)
						{
							const Pixel c2 = *(dest + addr + x);
							*(dest + addr + x) = AddBlend(c1, c2);
						}
					}
				}
				else
				{
					xs = (lsx > x1) ? lsx - x1 : 0;
					for (int x = xs; x < width; x++)
					{
						const Pixel c1 = *(src + x);
						if (c1 & 0xffffff) *(dest + addr + x) = c1;
					}
				}
				addr += dpitch;
				src += m_Pitch;
			}
		}
	}

	void Sprite::DrawScaled(int a_X, int a_Y, int a_Width, int a_Height, Surface* a_Target)
	{
		if ((a_Width == 0) || (a_Height == 0)) return;
		for (int x = 0; x < a_Width; x++) for (int y = 0; y < a_Height; y++)
		{
			int u = (int)((float)x * ((float)m_Width / (float)a_Width));
			int v = (int)((float)y * ((float)m_Height / (float)a_Height));
			Pixel color = GetBuffer()[u + v * m_Pitch];
			if (color & 0xffffff) a_Target->GetBuffer()[a_X + x + ((a_Y + y) * a_Target->GetPitch())] = color;
		}
	}

	void Sprite::InitializeStartData()
	{
		for (unsigned int f = 0; f < m_NumFrames; ++f)
		{
			m_Start[f] = new unsigned int[m_Height];
			for (int y = 0; y < m_Height; ++y)
			{
				m_Start[f][y] = m_Width;
				Pixel* addr = GetBuffer() + f * m_Width + y * m_Pitch;
				for (int x = 0; x < m_Width; ++x)
				{
					if (addr[x])
					{
						m_Start[f][y] = x;
						break;
					}
				}
			}
		}
	}

	Font::Font(const char* a_File, char* a_Chars)
	{
		m_Surface = new Surface(a_File);
		Pixel* b = m_Surface->GetBuffer();
		int w = m_Surface->GetWidth();
		int h = m_Surface->GetHeight();
		unsigned int charnr = 0, start = 0;
		m_Trans = new int[256];
		memset(m_Trans, 0, 1024);
		unsigned int i;
		for (i = 0; i < strlen(a_Chars); i++) m_Trans[(unsigned char)a_Chars[i]] = i;
		m_Offset = new int[strlen(a_Chars)];
		m_Width = new int[strlen(a_Chars)];
		m_Height = h;
		m_CY1 = 0, m_CY2 = 1024;
		int x, y;
		bool lastempty = true;
		for (x = 0; x < w; x++)
		{
			bool empty = true;
			for (y = 0; y < h; y++) if (*(b + x + y * w) & 0xffffff)
			{
				if (lastempty) start = x;
				empty = false;
			}
			if ((empty) && (!lastempty))
			{
				m_Width[charnr] = x - start;
				m_Offset[charnr] = start;
				if (++charnr == strlen(a_Chars)) break;
			}
			lastempty = empty;
		}
	}

	Font::~Font()
	{
		delete m_Surface;
		delete m_Trans;
		delete m_Width;
		delete m_Offset;
	}

	int Font::Width(const char* a_Text)
	{
		int w = 0;
		unsigned int i;
		for (i = 0; i < strlen(a_Text); i++)
		{
			unsigned char c = (unsigned char)a_Text[i];
			if (c == 32) w += 4; else w += m_Width[m_Trans[c]] + 2;
		}
		return w;
	}

	void Font::Centre(Surface* a_Target, char* a_Text, int a_Y)
	{
		int x = (a_Target->GetPitch() - Width(a_Text)) / 2;
		Print(a_Target, a_Text, x, a_Y);
	}

	void Font::Print(Surface* a_Target, char* a_Text, int a_X, int a_Y, bool clip)
	{
		Pixel* b = a_Target->GetBuffer() + a_X + a_Y * a_Target->GetPitch();
		Pixel* s = m_Surface->GetBuffer();
		unsigned int i, cx;
		int x, y;
		if (((a_Y + m_Height) < m_CY1) || (a_Y > m_CY2)) return;
		for (cx = 0, i = 0; i < strlen(a_Text); i++)
		{
			if (a_Text[i] == ' ') cx += 4; else
			{
				int c = m_Trans[(unsigned char)a_Text[i]];
				Pixel* t = s + m_Offset[c], * d = b + cx;
				if (clip)
				{
					for (y = 0; y < m_Height; y++)
					{
						if (((a_Y + y) >= m_CY1) && ((a_Y + y) <= m_CY2))
						{
							for (x = 0; x < m_Width[c]; x++)
								if ((t[x]) && ((x + (int)cx + a_X) < a_Target->GetPitch()))
									d[x] = AddBlend(t[x], d[x]);
						}
						t += m_Surface->GetPitch(), d += a_Target->GetPitch();
					}
				}
				else
				{
					for (y = 0; y < m_Height; y++)
					{
						if (((a_Y + y) >= m_CY1) && ((a_Y + y) <= m_CY2))
							for (x = 0; x < m_Width[c]; x++) if (t[x]) d[x] = AddBlend(t[x], d[x]);
						t += m_Surface->GetPitch(), d += a_Target->GetPitch();
					}
				}
				cx += m_Width[c] + 2;
				if ((int)(cx + a_X) >= a_Target->GetPitch()) break;
			}
		}
	}

}; // namespace Tmpl8