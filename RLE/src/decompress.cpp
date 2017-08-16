#include "decompress.hpp"

void decompress(uchar compressed[IN_SIZE], uchar decompressed[OUT_SIZE], int in_size, int &out_size)
{
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=compressed
#pragma HLS INTERFACE s_axilite port=decompressed
#pragma HLS INTERFACE s_axilite port=in_size
#pragma HLS INTERFACE s_axilite port=out_size

	// Internal variables
	uchar 	character = 0;
	int 	o_size	  = 0;
	// State
	state_t state = Char;

	MAIN_LOOP: for (int i=0; i<in_size; i++)
	{
		// Read a character
		uchar c = compressed[i];

		// When find ESCAPE character change state to Escape and continue
		if (c == ESCAPE && state == Char) state = Escape;

		else
		{
			switch (state)
			{
			// If current state is escape, this character must be char, and next state must be count
			case Escape:
				state = Count;
				character = c;
				break;

			// If current state is char, then nothing needs to be decompressed
			case Char:
				decompressed[o_size++] = c;
				character = c;
				break;

			// If current state is count, next state is char unless found escape
			case Count:
				// Convert count to integer
				int num = c;
				for (int i=0; i<num; i++) {
					decompressed[o_size++] = character;
				}
				state = Char;

			}
		}
	}

	for (int i=o_size; i<OUT_SIZE; i++) {
		decompressed[i] = (uchar)0;
	}

	// Output local out size to port out size
	out_size = o_size;
}
