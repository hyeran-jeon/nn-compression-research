#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "decompress.hpp"
#include "globals.hpp"
using namespace std;

#define LINE_SIZE 	40

#define ORIGINAL	"5-0.bin"
#define ENCODED		"5-0-e.bin"
#define DECODED		"5-0-d.bin"

bool compare(fstream &a, fstream &b);
void compare_files();

int main()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/* VARIABLES */

	uchar 	in[120000]  = {0};
	uchar 	out[150000] = {0};

//	for (int i=0; i<300000; i++) {
//		in[i]  = 0;
//		out[i] = 0;
//	}

	int in_size	 = 0;
	int out_size = 0;

	STREAM i_stream("IN_STREAM");
	STREAM o_stream("OUT_STREAM");

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/* EXTRACT INPUT FILE */

	fstream in_file(ENCODED,  fstream::in  | fstream::binary);
	fstream out_file(DECODED, fstream::out | fstream::binary | fstream::trunc);

	while (!in_file.eof())
	{
		in[in_size++] = in_file.get();
	}

//	/////////////////////////////////////////////////////////////////////////////////////////////////////
//	/* STREAM DECOMPRESSOR */
//
//	AXI_VALUE temp;
//
//	for (int i=0; i<in_size-1; i++) {
//		temp.data = (unsigned char)in[i];
//		temp.last = 0;
//		i_stream << temp;
//	}
//
//	// Insert a TLAST into input stream
//	AXI_VALUE tlast;
//	tlast.data = 0xFF;
//	tlast.last = 1;
//	i_stream << tlast;
//	in_size++;
//
//	/* CALL DECOMPRESSOR */
//
//	decompress(i_stream, o_stream, in_size, out_size);
//
//	/* READ FROM OUTPUT STREAM */
//
//	int out_index = 0;
//
//	while (!o_stream.empty()) {
//		o_stream >> temp;
//		out[out_index++] = temp.data;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////////////////
	/* BRAM DECOMPRESSOR */

	bram_decompress(in, out, in_size, out_size);

	/*OUTPUT TO FILE*/

	cout << "In  Size: " << in_size  << endl;
	cout << "Out Size: " << out_size << endl;

	cout << "Printing input:" << endl;
	for (int i=0; i<100; i++) {
		cout << hex << uppercase << static_cast<int>(in[i]) << " ";
	}

	cout << endl << "Printing output:" << endl;
	for (int i=0; i < out_size-1; i++) {
		out_file << out[i];
		if (i < 100) cout << hex << uppercase << static_cast<int>(out[i]) << " ";
	}
	cout << endl << endl;

	in_file.close();
	out_file.close();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/*COMPARE DECODED TO ORIGINAL FILE*/

	compare_files();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

bool compare(fstream &a, fstream &b)
{
	string c, d;
	int line_num = 0;

	while (!a.eof())
	{
		getline(a, c);	// a >> c
		getline(b, d);	// b >> d

		if (c.compare(d) != 0) {

			cout << "LINE NUMBER: " << line_num << endl;

			cout << "decoded line: ";
			for (int i=0; i<c.length(); i++) {
				cout << hex << uppercase << static_cast<int>(c[i]);
			}
			cout << endl;

			cout << "original line: ";
			for (int i=0; i<d.length(); i++) {
				cout << hex << uppercase << static_cast<int>(d[i]);
			}
			cout << endl;

			return false;
		}

		line_num++;
	}

	if (a.eof() && !b.eof()) return false;	// a ended but b didnt

	return true;
}

void compare_files()
{
	fstream decoded(DECODED, 	fstream::in);
	fstream original(ORIGINAL,  fstream::in);

	cout << ( (compare(decoded, original)) ? "Simulation passed!" : "Simulation failed!" )
		 << endl;

	decoded.close();
	original.close();
}



