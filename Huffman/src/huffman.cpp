#include "huffman.hpp"

void huffman(uchar code_lengths[HEX_COUNT], uchar compressed[MAX_CSIZE], uchar decompressed[MAX_DSIZE],
				int c_size, int &d_size)
{
// AXI LITE
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=code_lengths
#pragma HLS INTERFACE s_axilite port=compressed
#pragma HLS INTERFACE s_axilite port=decompressed
#pragma HLS INTERFACE s_axilite port=c_size
#pragma HLS INTERFACE s_axilite port=d_size

	/********************************************************************************************
	 * Originally wanted the decode to be a function of HuffmanTree but co-simulation produced
	 * errors doing that.  It might be because it is passing the input/output arrays between
	 * functions and HLS doesn't like that. Therefore the decode function is below.
	 ********************************************************************************************/

	// Clear everything
	for (int i=0; i<MAX_DSIZE; i++) decompressed[i] = 0;

	// Create a HuffmanTree object
	HuffmanTree tree;

	// Convert code lengths to codes
	tree.DecodeCodebook(code_lengths);

	// Local variables
	int CodeTable[512];
	for (int i=0; i<512; i++) {
		CodeTable[i] = -1;
	}

	// Grab CodeTable
	tree.ExportCodeTable(CodeTable);

	/////////////////////////////////////////////////////////////////////////////////////////////

	// For tracking indices of input/output arrays
	int in_index  = 0;
	int out_index = 0;

	// For storing overflow since we are parsing non-8-bit aligned codes
	int bits 		= 0;
	int accumulator = 0;

	int code 	= 0;			// Current code
	int itr 	= 0;			// Debug
	int num 	= 0;			// Last byte, number of remaining bits
	int loop_limit = MAXBITS;	// While loop parameter for when to stop

	// Main loop
	while (in_index < c_size)	// Last 2 bytes are special + EOF
	{
		// Grab byte
		accumulator <<= 8;
		accumulator |= compressed[in_index++];
		bits += 8;

		// Special care for the last 2 bytes, which are the remaining bits + number of remaining bits
		// For example 1101_0000 0000_0101 = take the MSB 5 bits from 1101_0000 = [11010]
		if (in_index == c_size - 2)
		{
			// Last byte, number of remaining bits
			num = compressed[in_index];

			// Discard extra bits
			accumulator >>= (8 - num);
			bits -= (8 - num);

			// Change loop limit
			loop_limit = 1;

			// Increment in_index so it breaks loop after this iteration
			in_index += 2;
		}

		// Decode as long as there are more bits than the longest code
		while (bits >= loop_limit)	// loop_limit changes on the last 2 bytes
		{
			// Parse bits
			for (int i=(bits-1); i>=0; i--)
			{
				// Shift code left to make room, then add in a bit i from accumulator
				code = (code << 1) | ((accumulator & (1 << i)) >> i);

				// Lookup code and if it exists, stop
				if (CodeTable[code] != -1 && CodeTable[code] != 511)
				{
					// Output code
					decompressed[out_index++] = CodeTable[code];
					// Delete bits from accumulator
					accumulator <<= (32 - bits + (bits -1 - i));	// Shift left to delete bits
					accumulator >>= (32 - bits + (bits -1 - i));
					// Decrease bits
					bits -= (bits - i);
					// Reset code
					code = 0;
					// If outputted code, restart while loop
					break;
				}
			}
		}
		itr++;
	}

	// Copy to output port
	d_size = out_index;
}
