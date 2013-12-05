#ifndef _BITMAP_
#define _BITMAP_

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

#pragma pack(push)
#pragma pack(1)
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

typedef struct __RGB_COLOR {
	BYTE r;
	BYTE g;
	BYTE b;
} RGB_COLOR, *PRGB_COLOR;

typedef struct __BITMAPHEADER {
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER inf;
} BITMAPHEADER, *PBITMAPHEADER;

typedef struct __BITMAP {
	BITMAPHEADER bmHeader;

	DWORD bmPaletteLength;
	PRGB_COLOR *bmPalette;

	DWORD bmMatrixWidth;
	DWORD bmMatrixHeight;
	BYTE *bmMatrixBmp;
} BITMAP, *PBITMAP;
#pragma pack(pop)

int read_bmp(const char *, PBITMAP);
int read_bmp_palette(FILE *, PBITMAP);
int read_bmp_matrix(FILE *, PBITMAP);
PBITMAP assign_filter(BITMAP, const float *filter, int, int);
BYTE to_byte(float);
int write_bmp(const char *, BITMAP);

#endif