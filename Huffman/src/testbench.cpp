#include <fstream>
#include <vector>
#include <string>
#include "huffman.hpp"

// Change these to reflect the files
#define CFILE  	"5-0-c.bin"
#define DFILE	"5-0-d.bin"
#define	OFILE	"5-0.bin"

void printCodeBits(int code, int length);

int main()
{

	// Open input and output files
	fstream ins(CFILE, 		fstream::in  | fstream::binary);
	fstream outs(DFILE, 	fstream::out | fstream::binary | fstream::trunc);
	fstream orig(OFILE,		fstream::in  | fstream::binary);

	// Arrays to hold input and output bytes
	uchar 	code_lengths[HEX_COUNT]	= {0};
	uchar 	compressed[MAX_CSIZE]	= {0};
	uchar 	decompressed[MAX_DSIZE]	= {0};

	// Index trackers
	int c_size = 0;
	int d_size = 0;

	// Read code book
	for (int i=0; i<HEX_COUNT; i++) {
		code_lengths[i] = ins.get();
	}

	while (!ins.eof()) compressed[c_size++] = ins.get();

	//////////////////////////////////////////////////////////////////////////////////////

	int codebook[256], codelengths[256], CodeTable[512];
	// Run decompressor
	huffman(code_lengths, compressed, decompressed, c_size, d_size);

	//////////////////////////////////////////////////////////////////////////////////////

	// Compare decoded to original
	vector <uchar> uncompressed;
	while (!orig.eof()) uncompressed.push_back(orig.get());
	uncompressed.pop_back();	// Pop EOF character

	bool pass = true;

	cout << "-------------------------------------------------------" 	<< endl;

	int count = 0;
	for (int i=0; i<uncompressed.size(); i++) {
		if (uncompressed[i] != decompressed[i]) {
			count++;
			pass = false;
		}
	}

	cout << "-------------------------------------------------------" 	<< endl;

	for (int i=0; i<64; i++) {
		if (i % 32 == 0) cout << endl;
		cout << hex << setw(2) << (int)decompressed[i] << " ";
	}
	cout << dec << endl;

	//////////////////////////////////////////////////////////////////////////////////////

	cout << "-------------------------------------------------------" 	<< endl;
	cout << "Uncompressed size: " 	<< uncompressed.size() 				<< endl;
	cout << "Decompressed size: " 	<< d_size 							<< endl;
	cout << "Simulation " 			<< (pass ? "passed!" : "failed!") 	<< endl;
	cout << "-------------------------------------------------------" 	<< endl;

	ins.close();
	outs.close();
	orig.close();
}

void printCodeBits(int code, int length)
{
	for (int b=(length-1); b>=0; b--)
	{
		if (code & (1 << b)) cout << "1";

		else cout << "0";
	}

	cout << endl;
}
