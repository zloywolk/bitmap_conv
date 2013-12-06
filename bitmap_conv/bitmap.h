#ifndef _BITMAP_
#define _BITMAP_

#define __IS_ARRAY		1

#pragma pack(push)
#pragma pack(1)

typedef unsigned int	DWORD;
typedef unsigned short	WORD;
typedef long			LONG;
typedef unsigned char	BYTE;

#define BI_RGB				0	/*�������� �����������*/
#define BI_RLE8				1	/*������ RLE ��� 8-������ �����������*/
#define BI_RLE4				2	/*������ RLE ��� 4-������ �����������*/
#define BI_BITFIELDS		3	/*����������� �� �����, ������� �������� ��� 4-������� ����� ��� �������, ������ � ����� ��������� �����. ������������ ��� 16- � 32-������ �����������*/
#define BI_JPEG				4	/*Win98/Me/2000/XP: JPEG-������*/
#define BI_PNG				5	/*Win98/Me/2000/XP: PNG-������*/
#define BI_ALPHABITFIELDS	6	/*WinCE: ����������� �� �����, ������� �������� ������ 4-������� ����� ��� �������, ������, ����� � ���������� (�����-�����) ��������� �����. ������������ ��� 16- � 32-������ �����������*/

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
#if __IS_ARRAY
	BYTE *bmMatrixBmp;
#else
	PRGB_COLOR *bmMatrixBmp;
#endif
} BITMAP, *PBITMAP;

#pragma pack(pop)

/* === FUNCTIONS PROTOTYPES === */

int read_bmp(const char *, PBITMAP);
int read_bmp_palette(FILE *, PBITMAP);
int read_bmp_matrix(FILE *, PBITMAP);
PBITMAP apply_filter(BITMAP, const float *filter, int, int);
BYTE to_byte(float);
int write_bmp(const char *, BITMAP);
void clear_shared_memory(PBITMAP *);

#endif