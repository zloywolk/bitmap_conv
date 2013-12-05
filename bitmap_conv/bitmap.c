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

	/* читаем заголовок BMP файла */
	i = fread_s(&bmp->bmHeader, sizeof(BITMAPHEADER), sizeof(BITMAPHEADER), 1, fd);
	bmp->bmPaletteLength = bmp->bmHeader.bmfh.bfOffBits - sizeof(BITMAPHEADER);

	if (!bmp->bmPaletteLength)
		bmp->bmPalette = NULL;
	else
		read_bmp_palette(fd, bmp);

	bmp->bmMatrixWidth = bmp->bmHeader.inf.biWidth;
	bmp->bmMatrixHeight = labs(bmp->bmHeader.inf.biHeight);

	/* чтаем все содержимое BMP файла после заголовка и палитры */
	read_bmp_matrix(fd, bmp);

	fclose(fd);

	return 0;
}

int read_bmp_palette(FILE *fd, PBITMAP bmp) {}

int read_bmp_matrix(FILE *fd, PBITMAP bmp) {
	int i, x, y;
	RGB_COLOR buffer;

	if (!fd) {
		printf("File not openned\n");
		return -1;
	}

	if(!bmp) {
		printf("Incorrect bitmap structure\n");
		return -1;
	}

	fseek(fd, bmp->bmHeader.bmfh.bfOffBits, SEEK_SET);
	bmp->bmMatrixBmp = (BYTE *)malloc(bmp->bmMatrixWidth * bmp->bmMatrixHeight * sizeof(RGB_COLOR));

	i = fread_s(bmp->bmMatrixBmp, bmp->bmMatrixWidth * bmp->bmMatrixHeight * sizeof(RGB_COLOR), sizeof(BYTE), bmp->bmMatrixWidth * bmp->bmMatrixHeight * sizeof(RGB_COLOR), fd);

	return 0;
}

PBITMAP assign_filter(BITMAP bmp, const float *filter, int f_width, int f_height) {
	int i, j, x, y, px, py;
	float sum_r, sum_g, sum_b;
	float sum_f, f_value;

	int is_border = 0;

	PBITMAP new_bmp = (PBITMAP)malloc(sizeof(BITMAP));
	
	/* копирование заголовка в новую структуру */
	new_bmp->bmHeader = bmp.bmHeader;
	new_bmp->bmMatrixHeight = bmp.bmMatrixHeight;
	new_bmp->bmMatrixWidth = bmp.bmMatrixWidth;
	new_bmp->bmPaletteLength = bmp.bmPaletteLength;

	new_bmp->bmMatrixBmp = (BYTE *)malloc(bmp.bmMatrixWidth * bmp.bmMatrixHeight * sizeof(RGB_COLOR));

	/* применение искомого фильтра */
	for (x = 0; x < bmp.bmMatrixWidth; ++x) {
		for(y = 0; y < bmp.bmMatrixHeight; ++y) {

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

						sum_r += (float)(*(bmp.bmMatrixBmp + sizeof(RGB_COLOR) * (bmp.bmMatrixWidth * py + px) + 0)) * f_value;
						sum_g += (float)(*(bmp.bmMatrixBmp + sizeof(RGB_COLOR) * (bmp.bmMatrixWidth * py + px) + 1)) * f_value;
						sum_b += (float)(*(bmp.bmMatrixBmp + sizeof(RGB_COLOR) * (bmp.bmMatrixWidth * py + px) + 2)) * f_value;

						sum_f += f_value;
				}

				
			}

			sum_f = (sum_f <= 0) ? 1.0f : sum_f;

			*(new_bmp->bmMatrixBmp + sizeof(RGB_COLOR) * (new_bmp->bmMatrixWidth * y + x) + 0) = to_byte(sum_r / sum_f);
			*(new_bmp->bmMatrixBmp + sizeof(RGB_COLOR) * (new_bmp->bmMatrixWidth * y + x) + 1) = to_byte(sum_g / sum_f);
			*(new_bmp->bmMatrixBmp + sizeof(RGB_COLOR) * (new_bmp->bmMatrixWidth * y + x) + 2) = to_byte(sum_b / sum_f);

		}
	}

	return new_bmp;
}

int write_bmp(const char *fname, BITMAP bmp) {
	int i, x, y;
	FILE *fd;

	if ((i = fopen_s(&fd, fname, "wb")) != 0) {
		printf("Cannt create file\n");
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
		printf("Палитра не поддерживается");
		return -2;
	}

	i = fwrite((void *)bmp.bmMatrixBmp, sizeof(BYTE), bmp.bmMatrixWidth * bmp.bmMatrixHeight * sizeof(RGB_COLOR), fd);

	fclose(fd);
	return 0;
}

BYTE to_byte(float f) {
	if (f < 0) return 0;
	else if (f > 255) return 255;
	else return f;
}