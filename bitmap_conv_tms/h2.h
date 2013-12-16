#ifndef _H2_H_
#define _H2_H_

#include "h1.h"

int *apply_filter(const int *bmp, const float* kernel, int b_width, int b_height, int c);
int to_byte(float f);

int *apply_filter(const int *bmp, const float* kernel, int b_width, int b_height, int c) {
	int *color = (int*)malloc(b_width * b_height);
	int x, y, i, j, px, py;
	float sum_c, sum_k, val_k;

	pdebug("K_WIDTH / 2 = %d\n", K_WIDTH / 2);
	
	for(x = 0; x < b_width; ++x) {
		for(y = 0; y < b_height; ++y) {
			for(i = 0, sum_c = .0f, sum_k = .0f; i < K_WIDTH; ++i) {
				for (j = 0; j < K_HEIGHT; ++j) {
					px = x + i - K_WIDTH / 2;
					py = y + j - K_HEIGHT / 2;
					
					if ((px < 0) || (px >= b_width) || (py < 0) || (py >= b_height))
						continue;

					val_k = *(kernel + i * K_HEIGHT + j);

					

					
					sum_c += (float)(*(bmp + COLOR_STRUCT_SIZE * (b_width * py + px) + c)) * val_k;
					sum_k += val_k;
				}
			}
		
			sum_k = (sum_k <= 0) ? 1.0f : sum_k;
			*(color + b_width * y + x) = (int)to_byte(sum_c / sum_k);		
		}
	}

	return color;
}

int to_byte(float f) {
	if (f < 0) return 0;
	else if (f > 255) return 255;
	else return (int)f;
}

#endif
