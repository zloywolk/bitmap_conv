#ifndef _BITMAP_
#define _BITMAP_

typedef unsigned int	DWORD;
typedef unsigned short	WORD;
typedef long			LONG;
typedef unsigned char	BYTE;
typedef unsigned char *	PBYTE;

#define BI_RGB				0	/*�������� �����������*/
#define BI_RLE8				1	/*������ RLE ��� 8-������ �����������*/
#define BI_RLE4				2	/*������ RLE ��� 4-������ �����������*/
#define BI_BITFIELDS		3	/*����������� �� �����, ������� �������� ��� 4-������� ����� ��� �������, ������ � ����� ��������� �����. ������������ ��� 16- � 32-������ �����������*/
#define BI_JPEG				4	/*Win98/Me/2000/XP: JPEG-������*/
#define BI_PNG				5	/*Win98/Me/2000/XP: PNG-������*/
#define BI_ALPHABITFIELDS	6	/*WinCE: ����������� �� �����, ������� �������� ������ 4-������� ����� ��� �������, ������, ����� � ���������� (�����-�����) ��������� �����. ������������ ��� 16- � 32-������ �����������*/

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

typedef struct __BITMAP {
	union __bmHeader {
		BITMAPFILEHEADER __bmfh;
		PBITMAPFILEHEADER __inf;
	} bmHeader;
	PRGB_COLOR *bmPalete;
	PRGB_COLOR *bmBody;
} BITMAP, *PBITMAP;

typedef struct __RGB_COLOR {
	BYTE b;
	BYTE g;
	BYTE r;
} RGB_COLOR, *PRGB_COLOR;
#pragma pack(pop)

#endif