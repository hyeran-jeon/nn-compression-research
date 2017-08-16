#include <iostream>
#include <iomanip>
#include <ap_int.h>
using namespace std;

typedef unsigned char uchar;
typedef ap_uint<16> int16;

#define HEX_COUNT 	256
#define MAXBITS	  	9
#define MAX_CSIZE	100000
#define MAX_DSIZE	150000

class HuffmanTree
{

public:

	// Constructor
	HuffmanTree();

	// Decodes the input bits into bit lengths to be converted into codes
	void	DecodeCodebook(uchar code_lengths[HEX_COUNT]);

	// Copies the CodeTable to an array passed into the function
	void 	ExportCodeTable(int table[512]);

	// Builds the look up table
	void	BuildHuffmanTables();

private:

	// Lookup code with byte
	int16	HuffmanCodes[HEX_COUNT];		// 256 * 4			= 1024 bytes

	// Bit lengths of each code
	int16	CodeLengths[HEX_COUNT];			// 256 * 4 			= 1024 bytes

	// Lookup byte with code
	int	CodeTable[1 << MAXBITS];		// 1<<9 = 512 * 4 	= 2048 bytes

											// 			Total	= 4096 bytes

};
