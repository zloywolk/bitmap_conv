#ifndef _H1_H_
#define _H1_H_


#include <stdio.h>
#include <stdarg.h>

#define BMPNAME 	"test1.bmp"
/*#define NBMPNAME 	"/student/k512b/k512b05/bitmap_conv/bitmap_conv_tms/n_test.bmp"*/
#define NBMPNAME 	"n_test.bmp"

#define K_WIDTH		3
#define K_HEIGHT 	3

#define BMP_HEADER_SIZE	0x000d
#define O_OFFSET_IMAGE_MATRIX 0x00002
#define O_OFFSET_IMAGE_WIDTH 0x0004
#define O_OFFSET_IMAGE_HEIGHT 0x0005
#define COLOR_STRUCT_SIZE 3

#define BYTE_SIZE	8
#define NUM_OF_CPUS	3

#define RED			0
#define	GREEN		1
#define	BLUE		2

#define __IS_SINGLE_CPU		1

int pdebug(const char *p, ...) {
	va_list arg;
	int r;

	va_start(arg, p);
	r = vfprintf(stderr, p, arg);
	va_end(arg);

	return r;
}

#endif
