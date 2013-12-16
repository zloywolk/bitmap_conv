#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <chan.h>
#include <math.h>

#include "h2.h"

int *create_rgb_matrix(int *bmp, int size);
int *create_bmp(int *r, int *g, int *b, int c_size);

main(argc, argv, envp, in_p, ins, out_p, outs)
int argc, ins, outs;
char *argv[], *envp[];
CHAN *in_p[], *out_p[];
{	
	int i, j;
	int *header = (int *)malloc(BMP_HEADER_SIZE * sizeof(int));
	/* ядра свертки */
	float kernel[K_HEIGHT * K_WIDTH] = {-1, -1, -1, -1, 9.0f, -1, -1, -1, -1}; /* sharp*/
	/*
	float kernel[K_HEIGHT * K_WIDTH] = {0, 1, 0, 1, -4, 1, 0, 1, 0};  laplas
	float kernel[K_HEIGHT * K_WIDTH] = {1, -2, 1, -2, 4, -2, 1, -2, 1}; laplas
	float kernel[K_HEIGHT * K_WIDTH] = {0, 1, 0, -1, 0, 1, 0, -1, 0}; тиснение
	float kernel[K_HEIGHT * K_WIDTH] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	float kernel[K_HEIGHT * K_WIDTH] = {-1, -1, -1, 0, 0, 0, 1, 1, 1}; 
	*/
	int *bmp = NULL, *new_bmp = NULL;
	int *red, *green, *blue;

	FILE *iop;

	int *bmp_temp;
	int t1, t2, t3, t4;
	int r, g, b;

	int width = 0, height = 0, actual_height = 0, offset = 0, image_size = 0, color_size, actual_size = 0;

	srand((unsigned int)time(NULL));

	t1 = timer_now();
	pdebug("Open a bmp file\n");
	if (!(iop = fopen(BMPNAME, "rb"))) {
		pdebug("Error openning file\n");
		exit(-1);
	}

	i = fread(header, sizeof(int), BMP_HEADER_SIZE, iop);

	width = *(header + O_OFFSET_IMAGE_WIDTH) >> 16;
	height = *(header + O_OFFSET_IMAGE_HEIGHT) >> 16;
	offset = *(header + O_OFFSET_IMAGE_MATRIX) >> 16;

	color_size = width * abs(height);
	image_size = COLOR_STRUCT_SIZE * color_size;
	actual_size = sizeof(int) * color_size / 4;

	bmp_temp = (int *)malloc(actual_size);
	fread((void *)bmp_temp, actual_size, 1, iop);
	fclose(iop);

	bmp = create_rgb_matrix(bmp_temp, actual_size);

#if !__IS_SINGLE_CPU
	red = (int *)malloc(color_size * sizeof(int));
	green = (int *)malloc(color_size * sizeof(int));
	blue = (int *)malloc(color_size * sizeof(int));

	for(i = 0; i < NUM_OF_CPUS; ++i)
	{
		chan_out_word(width, out_p[i]);
		chan_out_word(abs(height), out_p[i]);

		pdebug("H1: First byte of the BMP COLOR (RGB): %0x%02x\n", bmp[i]);
		chan_out_message(image_size * sizeof(char), (void *)bmp, out_p[i]);
		chan_out_message(K_WIDTH * K_HEIGHT * sizeof(float), (void *)kernel, out_p[i]);
		
		
		pdebug("H1 [%d]: KERNEL =>\n", i);
		for(j = 0; j < K_WIDTH * K_HEIGHT; ++j)
			pdebug("%s%.2f ", (!j) ? "H1: " : "" ,kernel[j]);

		pdebug("\n");
/*
		pdebug("H1: Recieve RED ARRAY\n");
		*/
		/*
		chan_in_message(color_size * sizeof(int), (void *)red, in_p[0]);
		if(red)
			pdebug("H1: First byte of the NEW RED: 0x%02x\n", *(red));
		*/
		/*if (i % NUM_OF_CPUS == 0) chan_in_word(&r, in_p[i]);
		if (i % NUM_OF_CPUS == 1) chan_in_word(&g, in_p[i]);
		if (i % NUM_OF_CPUS == 2) chan_in_word(&b, in_p[i]);*/
	}	

	chan_in_message(color_size * sizeof(int), (void *)red, in_p[0]);
	chan_in_message(color_size * sizeof(int), (void *)green, in_p[1]);
	chan_in_message(color_size * sizeof(int), (void *)blue, in_p[2]);

/*
	chan_in_word(&r, in_p[0]);
	chan_in_word(&g, in_p[1]);
	chan_in_word(&b, in_p[2]);
*/
/*
	pdebug("H1: Random byte of the NEW RED: 0x%02x\n", r);
	pdebug("H1: Random byte of the NEW GREEN: 0x%02x\n", g);
	pdebug("H1: Random byte of the NEW BLUE: 0x%02x\n", b);
*/
	r = rand() % actual_size;
	pdebug("H1: Random [%d] byte of the NEW RED: 0x%02x\n", r, red[r]);
	pdebug("H1: Random [%d] byte of the NEW GREEN: 0x%02x\n", r, green[r]);
	pdebug("H1: Random [%d] byte of the NEW BLUE: 0x%02x\n", r, blue[r]);
#else
	t3 = timer_now();
	red = apply_filter(bmp, kernel, width, abs(height), RED);
	green = apply_filter(bmp, kernel, width, abs(height), GREEN);
	blue = apply_filter(bmp, kernel, width, abs(height), BLUE);
	t4 = timer_now();
#endif

	new_bmp = create_bmp(red, green, blue, color_size);
	if(new_bmp) {
		pdebug("H1: Create a bmp file\n");
		if (!(iop = fopen(NBMPNAME, "wb"))) {
			pdebug("H1: Error create file\n");
			exit(-1);
		}	

		i = fwrite((void *)header, sizeof(int) * BMP_HEADER_SIZE, 1, iop);
		pdebug("Writed bytes: %d\n", BMP_HEADER_SIZE * i * 4);
		i = fwrite((void *)new_bmp, actual_size * sizeof(int), 1, iop);
		pdebug("Writed bytes: %d\n", actual_size * i * 4);

		fclose(iop);
		
		/*for(i = 0; i < BMP_HEADER_SIZE; ++i)
			fprintf(stderr, "%s", header[i]);
		fprintf(stderr, "\n");*/
	}
	
	t2 = timer_now();

	pdebug("width = %d\n", width);
	pdebug("height = %d\n", height);
	pdebug("offset = %d\n", offset);
	pdebug("image_size = %d\n", image_size);


	pdebug("Close the file\n");
	pdebug("t2 - t1 = %d ms\n", t2 - t1);

#if __IS_SINGLE_CPU
	pdebug("H1: Only BMP process: t4 - t3 = %d ms\n", t4 - t3);
#endif

	pdebug("SUCCESS\n");

	free(bmp);
	free(new_bmp);
	free(red);
	free(green);
	free(blue);
	free(header);

	exit(0);
}

int *create_rgb_matrix(int *bmp, int size) {
	int i;	
	int *rgb = (int *)malloc(size / sizeof(int) * 4 * COLOR_STRUCT_SIZE);

	for(i = 0; i < size; ++i) {
		/*if (i < 64) pdebug("0x%08x\n", bmp[i]);*/
		if(!i) {
			rgb[i * 4 + 1] = (bmp[i] & 0xff000000) >> 24;
			rgb[i * 4] = (bmp[i] & 0x00ff0000) >> 16;
		} else {
			rgb[i * 4 + 1] = (bmp[i] & 0xff000000) >> 24;
			rgb[i * 4] = (bmp[i] & 0x00ff0000) >> 16;
			rgb[i * 4 - 1] = (bmp[i] & 0x0000ff00) >> 8;
			rgb[i * 4 - 2] = bmp[i] & 0x000000ff;
		}	
	}

	/*
	for(i = 0; i < 64; ++i) {
		pdebug("RED [%d] = 0x%02x, GREEN [%d] = 0x%02x, BLUE [%d] = 0x%02x\n", i, *(rgb + i * 3 + RED), i, *(rgb + i * 3 + GREEN), i, *(rgb + i * 3 + BLUE));
	}
	*/		

	return rgb;
}


int *create_bmp(int *r, int *g, int *b, int c_size) {
	int i, j, t;
	int actual_size = sizeof(int) * c_size / 4;
	int *t_bmp = (int *)malloc((c_size * 3 + 2) * sizeof(int));
	int *bmp = (int *)malloc(actual_size * sizeof(int));

	t_bmp[0] = 0x00;
	t_bmp[1] = 0x00;

	for (i = 0; i < c_size; ++i) {
		t_bmp[i * 3 + 2 + RED] = r[i];
		t_bmp[i * 3 + 2 + GREEN] = g[i];
		t_bmp[i * 3 + 2 + BLUE] = b[i];
	}

	for(i = 0; i < c_size / 4; ++i) {
		bmp[i] = 0x00;
		bmp[i] += t_bmp[i * 4];		
		bmp[i] += t_bmp[i * 4 + 1] << 8;		
		bmp[i] += t_bmp[i * 4 + 2] << 16;		
		bmp[i] += t_bmp[i * 4 + 3] << 24;		

		/*if (i < 64) pdebug("0x%08x\n", bmp[i]);*/
	}

	return bmp;
}
