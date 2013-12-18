#include <stdio.h>
#include <time.h>

#include "bitmap.h"

#define	BMPNAME		"test.bmp"
#define	N_BMPNAME	"new_test.bmp"

#define F_WIDTH		3
#define	F_HEIGHT	3

// TODO: Начать разработку многопоточного решения

int main(int argc, char *argv[]) {
	int i;
	BITMAPIMAGE bmp, *new_bmp;

	DWORD t1, t2;

	/* ядра свертки */
	float kernel[F_HEIGHT * F_WIDTH] = {-1, -1, -1, -1, 9.0f, -1, -1, -1, -1}; // sharp
	//float kernel[F_HEIGHT * F_WIDTH] = {0, 1, 0, 1, -4, 1, 0, 1, 0}; // laplas
	//float kernel[F_HEIGHT * F_WIDTH] = {1, -2, 1, -2, 4, -2, 1, -2, 1}; // laplas
	//float kernel[F_HEIGHT * F_WIDTH] = {0, 1, 0, -1, 0, 1, 0, -1, 0}; // тиснение
	//float kernel[F_HEIGHT * F_WIDTH] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	//float kernel[F_HEIGHT * F_WIDTH] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};

	t1 = clock();
	if ((i = read_bmp(BMPNAME, &bmp)) != 0) {
		printf("Can\'t read a bmp file\n");
		return i;
	}

	printf("bmp[0x157] = %d\n", bmp.bmMatrixBmp[0x157 - 0x36]);

	new_bmp = apply_filter(bmp, kernel, F_WIDTH, F_HEIGHT);

	if(new_bmp)
		write_bmp(N_BMPNAME, *new_bmp);
	t2 = clock();

	printf("Image processed to %.2f ms\n", (((float)t2 - (float)t1) / CLOCKS_PER_SEC * 1000.0f));
	printf("Image saved to %s\n", N_BMPNAME);

	clear_shared_memory(&new_bmp);
	return 0;
}
