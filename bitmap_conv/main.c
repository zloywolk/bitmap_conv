#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#define _WINH	0

#if _WINH
#include <Windows.h>
#else
#include "bitmap.h"
#endif

#define	BMPNAME		"test1.bmp"
#define	N_BMPNAME	"new_test1.bmp"

#define F_WIDTH		3
#define	F_HEIGHT	3

main(int argc, char *argv[]) {
	int i;
	BITMAP bmp, *new_bmp;

	DWORD t1, t2;

	////float kernel[F_HEIGHT * F_WIDTH] = {-1, -1, -1, -1, 9.0f, -1, -1, -1, -1}; // sharp
	float kernel[F_HEIGHT * F_WIDTH] = {0, 1, 0, 1, -4, 1, 0, 1, 0}; // laplas
	//float kernel[F_HEIGHT * F_WIDTH] = {1, -2, 1, -2, 4, -2, 1, -2, 1}; // laplas
	//float kernel[F_HEIGHT * F_WIDTH] = {0, 1, 0, -1, 0, 1, 0, -1, 0}; // тиснение
	//float kernel[F_HEIGHT * F_WIDTH] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	
	//float kernel[F_HEIGHT * F_WIDTH] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};

	t1 = clock();
	if ((i = read_bmp(BMPNAME, &bmp)) != 0)
		exit(i);

	new_bmp = assign_filter(bmp, kernel, F_WIDTH, F_HEIGHT);

	if(new_bmp)
		write_bmp(N_BMPNAME, *new_bmp);
	t2 = clock();

	printf("Image processed to %.2f ms\nPress enter...", (((float)t2 - (float)t1) / CLOCKS_PER_SEC * 1000.0f));

	getchar();

	free(new_bmp->bmMatrixBmp);
	free(new_bmp);

	exit(0);
}