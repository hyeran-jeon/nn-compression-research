#include "huffman_decode.hpp"

HuffmanTree::HuffmanTree()
{
	for (int i=0; i<HEX_COUNT; i++) {
#pragma HLS PIPELINE II=1
		HuffmanCodes[i] = -1;
		CodeLengths[i]  = 0;
	}

	// Clear table
	for (int i=0; i<(1<<MAXBITS); i++) {
#pragma HLS PIPELINE II=1
		CodeTable[i] = -1;
	}
}

void HuffmanTree::BuildHuffmanTables()
{
	// Fill table
	for (int i=0; i<HEX_COUNT; i++)
	{
#pragma HLS PIPELINE II=1
		// Adding value=byte for key=code
		// Lookup by code, return byte
		// Then when looking up, look up in CodeLengths the length of the code
		if (HuffmanCodes[i] != -1) {
			CodeTable[HuffmanCodes[i]] = i;
		}
	}
}

void HuffmanTree::DecodeCodebook(uchar code_lengths[])
{
	// Array that corresponds to the each index of the code_lengths array
	uchar bytes[HEX_COUNT];
	for (int i=0; i<HEX_COUNT; i++) {
#pragma HLS PIPELINE II=1
		bytes[i] = i;
	}

	// Insertion Sort code lengths, while swapping entries in bytes[] simultaneously
	// This preserves the order of the codes with the same lengths
	int j, key_lengths, key_bytes;
	for (int i=1; i<HEX_COUNT; i++)
	{
		key_lengths = code_lengths[i];
		key_bytes	= bytes[i];
		j = i - 1;

		while (j >= 0 && code_lengths[j] > key_lengths)
		{
			code_lengths[j+1] = code_lengths[j];
			bytes[j+1] = bytes[j];
			j--;
		}

		code_lengths[j+1] = key_lengths;
		bytes[j+1] = key_bytes;
	}

	// Store lengths
	for (int i=0; i<HEX_COUNT; i++) {
#pragma HLS PIPELINE II=1
		CodeLengths[bytes[i]] = code_lengths[i];
	}

	// Find the index of the first code that has a length > 0
	int index_first_code = 0;
	for (int i=0; i<HEX_COUNT; i++) {
#pragma HLS PIPELINE II=1
		if (code_lengths[i] > 0) {
			index_first_code = i;
			break;
		}
	}

	// Generate canonical codes, in order of the previouly sorted array
	int code 	= -1;
	int length 	= 0;
	for (int i=index_first_code; i<HEX_COUNT; i++)
	{
#pragma HLS PIPELINE II=2
		if (i == index_first_code)
		{
			code = 0;
			length = code_lengths[i];
		}
		else
		{
			// If length is longer than the previous code
			if (code_lengths[i] > length) {
				code++;
				code = code << (code_lengths[i] - length);
			}
			else {
				code++;
			}
			length = code_lengths[i];
		}
		// Store code for all codes with length > 0
		HuffmanCodes[bytes[i]] = code;
	}

	BuildHuffmanTables();
}

void HuffmanTree::ExportCodeTable(int table[512])
{
	for (int i=0; i<512; i++)
	{
		table[i] = CodeTable[i];
	}
}
