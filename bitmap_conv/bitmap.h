#ifndef _BITMAP_
#define _BITMAP_

#define __BMP_AS_ARRAY		1


#if defined(WIN32) || defined(_WIN32) || defined(_W64)
#define	__WINDOWS__
#include <windows.h>
#elif defined(__linux__) || (__linux)
#define __LINUX__
#endif

#pragma pack(push)
#pragma pack(1)

#ifndef __WINDOWS__
typedef unsigned int	DWORD;
typedef unsigned short	WORD;
typedef long			LONG;
typedef unsigned char	BYTE;

#define BI_RGB				0	/*несжатое изображение*/
#define BI_RLE8				1	/*сжатие RLE для 8-битных изображений*/
#define BI_RLE4				2	/*сжатие RLE для 4-битных изображений*/
#define BI_BITFIELDS		3	/*изображение не сжато, палитра содержит три 4-байтные маски для красной, зелёной и синей компонент цвета. Используется для 16- и 32-битных изображений*/
#define BI_JPEG				4	/*Win98/Me/2000/XP: JPEG-сжатие*/
#define BI_PNG				5	/*Win98/Me/2000/XP: PNG-сжатие*/
#define BI_ALPHABITFIELDS	6	/*WinCE: изображение не сжато, палитра содержит четыре 4-байтные маски для красной, зелёной, синей и прозрачной (альфа-канал) компонент цвета. Используется для 16- и 32-битных изображений*/

typedef struct __BITMAPFILEHEADER {
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct __BITMAPINFOHEADER {
  DWORD  biSize; 
  LONG   biWidth;
  LONG   biHeight; 
  WORD   biPlanes; 
  WORD   biBitCount; 
  DWORD  biCompression; 
  DWORD  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  DWORD  biClrUsed; 
  DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#endif

typedef struct __RGB_COLOR {
	BYTE r;
	BYTE g;
	BYTE b;
} RGB_COLOR, *PRGB_COLOR;

typedef struct __BITMAPHEADER {
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER inf;
} BITMAPHEADER, *PBITMAPHEADER;

typedef struct __BITMAPIMAGE {
	BITMAPHEADER bmHeader;

	DWORD bmPaletteLength;
	PRGB_COLOR *bmPalette;

	DWORD bmMatrixWidth;
	DWORD bmMatrixHeight;
#if __BMP_AS_ARRAY
	BYTE *bmMatrixBmp;
#else
	PRGB_COLOR *bmMatrixBmp;
#endif
} BITMAPIMAGE, *PBITMAPIMAGE;

#pragma pack(pop)

/* === function prototypes === */

int read_bmp(const char *, PBITMAPIMAGE);
int read_bmp_palette(FILE *, PBITMAPIMAGE);
int read_bmp_matrix(FILE *, PBITMAPIMAGE);
PBITMAPIMAGE apply_filter(BITMAPIMAGE, const float *filter, int, int);
BYTE to_byte(float);
int write_bmp(const char *, BITMAPIMAGE);
void clear_shared_memory(PBITMAPIMAGE *);

#endif
