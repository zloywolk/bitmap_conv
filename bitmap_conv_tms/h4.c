#include <stdio.h>
#include <chan.h>

#include "h2.h"

main(argc, argv, envp, in_p, ins, out_p, outs)
int argc, ins, outs;
int *argv[], *envp[];
CHAN *in_p[], *out_p[];
{
	int i;
	int *bmp, *color = NULL;
	float kernel[K_WIDTH * K_HEIGHT];

	int t1, t2, t3, t4;

	int image_size, width, height, color_size;
#if !__IS_SINGLE_CPU 
	pdebug("H4: Recieve & process sizes of image and arrays\n");
	chan_in_word(&width, in_p[0]);
	chan_in_word(&height, in_p[0]);

	image_size = COLOR_STRUCT_SIZE * width * height;
	color_size = width * height;

	pdebug("H4: image_size = COLOR_STRUCT_SIZE * width * height = %d * %d * %d = %d\n",
		 COLOR_STRUCT_SIZE, width, height, image_size);
	pdebug("H4: color_size = width * height = %d * %d = %d\n", width, height, color_size);

	pdebug("H4: Recieve bmp color array\n");
	bmp = (int *)malloc(image_size * sizeof(int));
	chan_in_message(image_size * sizeof(int), (void *)bmp, in_p[0]);
	
	pdebug("H4: Recieve kernel\n");
	chan_in_message(K_WIDTH * K_HEIGHT * sizeof(float), (void *)kernel, in_p[0]);

	for(i = 0; i < K_WIDTH * K_HEIGHT; ++i)
		pdebug("%s%.2f ", (!i) ? "H4: " : "" ,kernel[i]);
	pdebug("\n");

	t3 = timer_now();
	color = apply_filter(bmp, kernel, width, height, BLUE);
	t4 = timer_now();
	pdebug("H4: Process BLUE COLOR: t4 - t3 = %d ms\n", t4 - t3);
	if(color)
		pdebug("H4: First byte of the NEW BLUE: 0x%02x\n", *(color));

	pdebug("H4: Send BLUE ARRAY\n");

	if(color)
		chan_out_message(color_size * sizeof(int), (void *)color, out_p[0]);

/*
	if(color)
		chan_out_word(color[0], out_p[0]);
*/
	free(bmp);
	free(color);
#endif
}

