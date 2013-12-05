#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define _WINH	0

#if _WINH
#include <Windows.h>
#else
#include "bitmap.h"
#endif

#define KERNEL_A	3
#define KERNEL_B	3

#define AB	1 /*/(KERNEL_A * KERNEL_B)*/

#define	R	2
#define	G	1
#define	B	0

unsigned char **get_color_matrix(FILE *fd, LONG width, LONG height, DWORD offset);
void save_bitmap(const char *fname, BITMAPFILEHEADER bmfh, BITMAPINFOHEADER info, unsigned char **bitmap);

main(int argc, char *argv[]) {
	FILE *fd;
	int x, y;
	int i, j, k;

	LONG width, height;

	float r, g, b;
	float new_r, new_g, new_b;


	float sumr, sumg, sumb, sumk;

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER infh;

	unsigned char *pixel = (unsigned char *)malloc(3);
	unsigned char **bmp, **new_bmp;

	char *fname = "test.bmp";

	float kernel[][3] = {0.045, 0.122, 0.045, 0.122, 
  0.332, 0.122, 0.045, 0.122, 0.045};

	if ((i = fopen_s(&fd, fname,  "rb")) != 0) {
		printf("Error open bitmap\n");
		exit(1);
	} 

	i = fread_s(&bmfh, sizeof(BITMAPFILEHEADER), sizeof(BITMAPFILEHEADER), 1, fd);
	if ((i = fread_s(&infh, sizeof(BITMAPINFOHEADER), sizeof(BITMAPINFOHEADER), 1, fd))) {
		height = labs(infh.biHeight);
		width = infh.biWidth;
	}

	bmp = get_color_matrix(fd, width, height, bmfh.bfOffBits);
	new_bmp = (unsigned char **)malloc(3 * width * height * sizeof(unsigned char *));

	for(x = 0; x < width; ++x) {
		for (k = 0; k < 3; ++k)
			new_bmp[3 * x + k] = (unsigned char *)malloc(height * sizeof(unsigned char));

		for(y = 0; y < height; ++y) {
			if ((x > 0 && y > 0) && (x < width - 1 && y < height - 1)) {
				sumr = sumg = sumb = sumk = 0.0f;

				for (i = 0; i < KERNEL_A; ++i) {
					for (j = 0; j < KERNEL_B; ++j) {
						sumr += kernel[i][j] * (1.0f / 255.0f) * bmp[x - 1 + i + R][y - 1 + j];
						sumg += kernel[i][j] * (1.0f / 255.0f) * bmp[x - 1 + i + G][y - 1 + j];
						sumb += kernel[i][j] * (1.0f / 255.0f) * bmp[x - 1 + i + B][y - 1 + j];
						sumk += kernel[i][j];
					}
				}

				if(sumk <= 0) sumk = 1;

				sumr /= sumk;
				sumg /= sumk;
				sumb /= sumk;

				new_bmp[3 * x + R][y] = (unsigned char)(sumr * 255.0f);
				new_bmp[3 * x + G][y] = (unsigned char)(sumg * 255.0f);
				new_bmp[3 * x + B][y] = (unsigned char)(sumb * 255.0f);
			} else {
				new_bmp[3 * x + R][y] = bmp[3 * x + R][y];
				new_bmp[3 * x + G][y] = bmp[3 * x + G][y];
				new_bmp[3 * x + B][y] = bmp[3 * x + B][y];
			}
		}
	}

	fclose(fd);
	save_bitmap("test_new.bmp", bmfh, infh, new_bmp);

	getchar();

	exit(0);
}

unsigned char **get_color_matrix(FILE *fd, LONG width, LONG height, DWORD offset) {
	int i;
	int x, y;

	unsigned char *pixel = (unsigned char *)malloc(3 * sizeof(unsigned char));

	unsigned char **out_image = (unsigned char **)malloc(3 * width * height * sizeof(unsigned char *));
	if(fd) {
		i = fseek(fd, offset, SEEK_SET);

		for (x = 0; x < 3 * width; ++x) {
			out_image[x] = (unsigned char *)malloc(height * sizeof(unsigned char));

			if (!((x + 1) % 3))
			{
				for (y = 0; y < height; ++y) {
					fread_s(pixel, 3 * sizeof(unsigned char), 3 * sizeof(unsigned char), 1, fd);

					if(pixel) {
						out_image[x - R][y] = pixel[B];
						out_image[x - G][y] = pixel[G];
						out_image[x - B][y] = pixel[R];
					}
				}
			}
		}
	} else return NULL;

	free(pixel);
	return out_image;
}

void save_bitmap(const char *fname, BITMAPFILEHEADER bmfh, BITMAPINFOHEADER info, unsigned char **bitmap) {
	FILE *fd;
	int i, x, y;

	DWORD offset = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	unsigned char *pixel = (unsigned char *)malloc(3 * sizeof(unsigned char));

	LONG width = info.biWidth;
	LONG height = labs(info.biHeight);

	assert(bitmap);

	if ((i = fopen_s(&fd, fname,  "wb")) != 0) {
		printf("Error open bitmap\n");
		exit(1);
	} 

	fwrite((char *)&bmfh, sizeof(BITMAPFILEHEADER), 1, fd);
	fwrite((char *)&info, sizeof(BITMAPINFOHEADER), 1, fd);

	if (offset) { /**/ }

	for (x = 0; x < width; ++x) {
		for (y = 0; y < height; ++y) {
			pixel[G] = bitmap[3 * x + G][y];
			pixel[B] = bitmap[3 * x + B][y];
			pixel[R] = bitmap[3 * x + R][y];

			fwrite((char *)pixel, 3 * sizeof(unsigned char), 1, fd);
		}
	}

	fclose(fd);
}