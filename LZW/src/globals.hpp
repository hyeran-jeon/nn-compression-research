#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_int.h>
using namespace std;

#define DICT_SIZE  4096	// Dict Size

typedef unsigned int  uint;
typedef unsigned char uchar;

typedef ap_axiu <8,1,1,1> 		AXI_VALUE;
typedef hls::stream<AXI_VALUE> 	STREAM;
typedef ap_int<32> 				MASTER;


struct elem
{
	int code;
	int byte;
	int prefix;
};

inline int readBits(STREAM &in, int &in_index, int in_size, int bits)
{
	int c = 0;
	AXI_VALUE temp;

	static int numOverflow 	= 0;
	static uint overflow 	= 0;

	while (numOverflow < bits)
	{
		// Check first if stream is empty to prevent hanging
		if (in_index == in_size) return -1;

		// Read from stream, increment index
		temp = in.read();
		in_index++;

		// If TLAST is asserted return immediately
		if (temp.last == 1) return -1;
		else 				c = temp.data;

		numOverflow += 8;
		overflow = (overflow << 8) | c;
	}

	numOverflow -= bits;
	c = overflow >> numOverflow;
	overflow = overflow ^ (c << numOverflow);

	return c;
}

inline int bram_readBits(uchar *in_file, int &in_index, int in_size, int bits)
{
	int c = 0;

	static int numOverflow 	= 0;
	static uint overflow 	= 0;

	while (numOverflow < bits)
	{
		if (in_index == in_size) return -1;

		c = in_file[in_index++];

		if (c == EOF) return -1;

		numOverflow += 8;
		overflow = (overflow << 8) | c;
	}

	numOverflow -= bits;
	c = overflow >> numOverflow;
	overflow = overflow ^ (c << numOverflow);

	return c;
}
