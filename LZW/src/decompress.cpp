#include "decompress.hpp"

void decompress(STREAM &in_stream, STREAM &out_stream, int in_size, int &out_size)
{
// AXI STREAM PORTS
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS STREAM variable=in_stream  depth=131072
#pragma HLS STREAM variable=out_stream depth=131072
// AXI LITE PORTS
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=in_size
#pragma HLS INTERFACE s_axilite port=out_size
// OPTIMIZATION
#pragma HLS INLINE

	int in_index  = 0;					// Current index of input stream
	int out_index = 0;					// Current index of output stream
	AXI_VALUE temp;						// AXI object to be outputted

	int code	= 0;					// Code for lookup
	int newcode	= 0;					// Copy of code
	int oldcode	= 0;					// Code from previous iteration
	int final	= 0;					// Char to be outputted
	int bits 	= 9;					// Number of bits code is
	int itr 	= 0;					// Iteration number
	int count	= 0;					// Number of elements currently in dictionary
	uchar c 	= 0;					// Hold char

	struct elem array[DICT_SIZE];		// Dictionary
	struct elem e;						// Hold element
	struct elem null_elem;				// Null element
	null_elem.code 	 = -1;
	null_elem.byte 	 = -1;
	null_elem.prefix = -1;

	stack s;							// Stack for storing elements

	// Initialize dictionary
	for (int i=0; i<DICT_SIZE; i++) {
		if (i < 4) {
			array[i].code   = count;
			array[i].byte   = -1;
			array[i].prefix = 0;
			count++;
		}
		else if (i < 260) {
			array[i].code   = count;	// Code is the number of arrival, 10th insertion = 10
			array[i].byte   = i-4;		// Byte is the number
			array[i].prefix = 0;		// Prefix is the previous code: oldcode
			count++;
		}
		else {
			array[i].code   = -1;
			array[i].byte   = -1;
			array[i].prefix = -1;
		}
	}

	// Initializes the correct overflow values before starting
	readBits(in_stream, in_index, in_size, 1);

	// When in_index == in_size, code should be set as EOF and break the loop
	while (code != EOF)
	{
		itr++;
		// read from input, convert to lookup code
		code = newcode = readBits(in_stream, in_index, in_size, bits);

		// end of file
		if (code == EOF) {
			temp.data = 0xFF;
			temp.last = 1;
			out_stream << temp;
			out_index++;
			break;
		}
		// increment bits
		else if (code == 3) { bits++; continue; }

		// lookup code
		if (code < DICT_SIZE) 	e = array[code];
		else 					e = null_elem;

		// element does not exist
		if (e.code == -1) {
			s.push(final);
			// lookup based on oldcode
			if (oldcode < DICT_SIZE) 	e = array[oldcode];
			else 						e = null_elem;
		}

		// element does not exist again
		if (e.code == -1) {
			cout << "CORRUPTED!" << endl;
			break;
		}

		// push to stack
		while (e.prefix != 0)
		{
			s.push(e.byte);
			code = e.prefix;
			// lookup oldcode of e, basically traces in the array
			// back until prefix=0 or a deadend, which is always one of the 0-255 ascii characters
			if (code < DICT_SIZE) 	e = array[code];
			else 					e = null_elem;
		}

		// last character that was not pushed to stack
		final = e.byte;
		temp.data = (unsigned char)final;
		temp.last = 0;
		out_stream << temp;
		out_index++;

		// pop from stack and output
		while (!s.empty())
		{
			c = (unsigned char)s.pop();
			temp.data = c;
			temp.last = 0;
			out_stream << temp;
			out_index++;
		}

		// insert if not first iteration, there is space, and the spot is empty
		if (oldcode != 0 && count < DICT_SIZE && array[count].code == -1) {
			struct elem f;
			f.code   = count;
			f.byte   = final;
			f.prefix = oldcode;
			array[count] = f;
			count++;
		}

		// store the current code as oldcode so it can be used in case the next code is not found in array
		oldcode = newcode;
	}

	// Copy value to output port
	out_size = out_index;
}
