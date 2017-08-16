#pragma once

#define IN_SIZE		(150000)
#define OUT_SIZE	(150000)
#define ESCAPE 		((unsigned char)0x01)

typedef unsigned char uchar;

typedef enum state { Escape, Char, Count } state_t;

void decompress(uchar compressed[IN_SIZE], uchar decompressed[OUT_SIZE], int in_size, int &out_size);
