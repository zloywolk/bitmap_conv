#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bitmap.h"

int read_bmp(const char *fname, PBITMAP bmp) {
	FILE *fd;
	int i;

	if ((i = fopen_s(&fd, fname, "rb")) != 0) {
		printf("Error open bitmap\n");
		return i;
	}

	/* ������ ��������� BMP ����� */
	i = fread_s(&bmp->bmHeader, sizeof(BITMAPHEADER), sizeof(BITMAPHEADER), 1, fd);
	bmp->bmPaletteLength = bmp->bmHeader.bmfh.bfOffBits - sizeof(BITMAPHEADER);

	if (!bmp->bmPaletteLength)
		bmp->bmPalette = NULL;
	else
		read_bmp_palette(fd, bmp);

	bmp->bmMatrixWidth = bmp->bmHeader.inf.biWidth;
	bmp->bmMatrixHeight = labs(bmp->bmHeader.inf.biHeight);

	/* ����� ��� ���������� BMP ����� ����� ��������� � ������� */
	read_bmp_matrix(fd, bmp);

	fclose(fd);

	return 0;
}

int read_bmp_palette(FILE *fd, PBITMAP bmp) { return 0; }

int read_bmp_matrix(FILE *fd, PBITMAP bmp) {
	int i;

#if !__IS_ARRAY
	DWORD y;
#endif

	if (!fd) {
		printf("File not openned\n");
		return -1;
	}

	if(!bmp) {
		printf("Incorrect bitmap structure\n");
		return -1;
	}

	fseek(fd, bmp->bmHeader.bmfh.bfOffBits, SEEK_SET);

#if __IS_ARRAY
	bmp->bmMatrixBmp = (BYTE *)malloc(bmp->bmMatrixWidth * bmp->bmMatrixHeight * sizeof(RGB_COLOR));
	i = fread_s(bmp->bmMatrixBmp, bmp->bmMatrixWidth * bmp->bmMatrixHeight * sizeof(RGB_COLOR), sizeof(BYTE), bmp->bmMatrixWidth * bmp->bmMatrixHeight * sizeof(RGB_COLOR), fd);
#else
	bmp->bmMatrixBmp = (PRGB_COLOR *)malloc(bmp->bmMatrixWidth * bmp->bmMatrixHeight * sizeof(PRGB_COLOR));
	for (y = 0; y < bmp->bmMatrixHeight; ++y) {
		bmp->bmMatrixBmp[y] = (PRGB_COLOR)malloc(bmp->bmMatrixWidth * sizeof(RGB_COLOR));
		i = fread_s(bmp->bmMatrixBmp[y], bmp->bmMatrixWidth * sizeof(RGB_COLOR), sizeof(RGB_COLOR), bmp->bmMatrixWidth, fd);
	}
#endif

	return 0;
}

PBITMAP apply_filter(BITMAP bmp, const float *filter, int f_width, int f_height) {
	DWORD x, y, px, py;
	int i, j;
	float sum_r, sum_g, sum_b;
	float sum_f, f_value;

	PBITMAP new_bmp = (PBITMAP)malloc(sizeof(BITMAP));
	
	/* ����������� ��������� � ����� ��������� */
	new_bmp->bmHeader = bmp.bmHeader;
	new_bmp->bmMatrixHeight = bmp.bmMatrixHeight;
	new_bmp->bmMatrixWidth = bmp.bmMatrixWidth;
	new_bmp->bmPaletteLength = bmp.bmPaletteLength;

#if __IS_ARRAY
	new_bmp->bmMatrixBmp = (BYTE *)malloc(bmp.bmMatrixWidth * bmp.bmMatrixHeight * sizeof(RGB_COLOR));
#else
	new_bmp->bmMatrixBmp = (PRGB_COLOR *)malloc(bmp.bmMatrixWidth * bmp.bmMatrixHeight * sizeof(PRGB_COLOR));
#endif

	/* ���������� �������� ������� */
	for (x = 0; x < bmp.bmMatrixWidth; ++x) {
		for(y = 0; y < bmp.bmMatrixHeight; ++y) {
#if !__IS_ARRAY
			if(!x)
				new_bmp->bmMatrixBmp[y] = (PRGB_COLOR)malloc(new_bmp->bmMatrixWidth * sizeof(RGB_COLOR));
#endif
			sum_r = .0f; sum_g = .0f; sum_b = .0f; sum_f = .0f;
			for(i = 0; i < f_width; ++i) {
				for(j = 0; j < f_height; ++j) {
					px = x + i - (f_width / 2);
					py = y + j - (f_height / 2);

					if((px < 0) || 
						(px >= new_bmp->bmMatrixWidth) ||
						(py < 0) || 
						(py >= new_bmp->bmMatrixHeight)) continue;

						f_value = *(filter + i * f_height + j);
#if __IS_ARRAY
						sum_r += (float)(*(bmp.bmMatrixBmp + sizeof(RGB_COLOR) * (bmp.bmMatrixWidth * py + px) + 0)) * f_value;
						sum_g += (float)(*(bmp.bmMatrixBmp + sizeof(RGB_COLOR) * (bmp.bmMatrixWidth * py + px) + 1)) * f_value;
						sum_b += (float)(*(bmp.bmMatrixBmp + sizeof(RGB_COLOR) * (bmp.bmMatrixWidth * py + px) + 2)) * f_value;
#else
						sum_r += bmp.bmMatrixBmp[py][px].r * f_value;
						sum_g += bmp.bmMatrixBmp[py][px].g * f_value;
						sum_b += bmp.bmMatrixBmp[py][px].b * f_value;
#endif

						sum_f += f_value;
				}
			}

			sum_f = (sum_f <= 0) ? 1.0f : sum_f;

#if __IS_ARRAY
			*(new_bmp->bmMatrixBmp + sizeof(RGB_COLOR) * (new_bmp->bmMatrixWidth * y + x) + 0) = to_byte(sum_r / sum_f);
			*(new_bmp->bmMatrixBmp + sizeof(RGB_COLOR) * (new_bmp->bmMatrixWidth * y + x) + 1) = to_byte(sum_g / sum_f);
			*(new_bmp->bmMatrixBmp + sizeof(RGB_COLOR) * (new_bmp->bmMatrixWidth * y + x) + 2) = to_byte(sum_b / sum_f);
#else
			new_bmp->bmMatrixBmp[y][x].r = to_byte(sum_r / sum_f);
			new_bmp->bmMatrixBmp[y][x].g = to_byte(sum_g / sum_f);
			new_bmp->bmMatrixBmp[y][x].b = to_byte(sum_b / sum_f);
#endif

		}
	}

	return new_bmp;
}

int write_bmp(const char *fname, BITMAP bmp) {
	int i;
#if !__IS_ARRAY
	DWORD y;
#endif
	FILE *fd;

	if ((i = fopen_s(&fd, fname, "wb")) != 0) {
		printf("Can\'t create file\n");
		return i;
	}

	if (!bmp.bmMatrixBmp) {
		printf("Incorrect bmp format");
		fclose(fd);
		return -1;
	}

	i = fwrite((void *)&bmp.bmHeader, sizeof(BITMAPHEADER), 1, fd);
	
	/* save content */
	if (bmp.bmPaletteLength) {
		printf("������� �� ��������������");
		return -2;
	}

#if __IS_ARRAY
	i = fwrite((void *)bmp.bmMatrixBmp, sizeof(BYTE), bmp.bmMatrixWidth * bmp.bmMatrixHeight * sizeof(RGB_COLOR), fd);
#else
	for (y = 0; y < bmp.bmMatrixHeight; ++y)
		i = fwrite((void *)bmp.bmMatrixBmp[y], sizeof(RGB_COLOR), bmp.bmMatrixWidth, fd);
#endif

	fclose(fd);
	return 0;
}

BYTE to_byte(float f) {
	if (f < 0) return 0;
	else if (f > 255) return 255;
	else return (BYTE)f;
}

void clear_shared_memory(PBITMAP *bmp) {
#if __IS_ARRAY
	free((*bmp)->bmMatrixBmp);
#else
	DWORD y;
	for ( y = 0; y < (*bmp)->bmMatrixHeight; ++y)
		free((*bmp)->bmMatrixBmp[y]);
#endif
	free(*bmp);
}
