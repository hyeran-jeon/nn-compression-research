#include "compress.hpp"
#include "decompress.hpp"
#include <cstdio>

int main()
{
	printf("Starting C Simulation...\n");

	fstream ins("5-0.bin", fstream::in | fstream::binary);
	fstream outs("5-0-c.bin", fstream::out | fstream::binary | fstream::trunc);

	// Compress first
	compress(ins, outs);

	ins.close();
	outs.close();

	printf("Finished Compression.\n");

	/////////////////////////////////////////////////////////////////////////////////////

	// Compressed file
	fstream c_file("5-0-c.bin", fstream::in | fstream::binary);
	// Original file
	fstream o_file("5-0.bin", fstream::in | fstream::binary);
	// Decompressed file
	fstream d_file("5-0-d.bin", fstream::out | fstream::binary);

	// Fetch all compressed data from compressed file
	vector <uchar> c_vec;
	while (!c_file.eof()) c_vec.push_back(c_file.get());
	if (c_vec.back() == EOF) c_vec.pop_back();

	// Fetch all data from original file
	vector <uchar> o_vec;
	while (!o_file.eof()) o_vec.push_back(o_file.get());
	if (o_vec.back() == EOF) o_vec.pop_back();

	uchar compressed[IN_SIZE] 	 = {0};
	uchar decompressed[OUT_SIZE] = {0};

	for (int i=0; i<150000; i++) {
		compressed[i] 	= (uchar)0;
		decompressed[i] = (uchar)0;
	}

	int in_size = 0;
	int out_size = 0;

	// Transfer compressed data from vector to uchar array
	for (int i=0; i<c_vec.size(); i++) {
		compressed[i] = c_vec[i];
	}
	in_size = c_vec.size()-1;

	// Decompress
	decompress(compressed, decompressed, in_size, out_size);

	printf("In Size:  %i\n", in_size);
	printf("Out Size: %i\n", out_size);
	printf("Original Size: %i\n", o_vec.size());

	// Verify data
	bool pass = true;
	for (int i=0; i<out_size; i++) {
		if (decompressed[i] != o_vec[i]) {
			printf("[%i] %i != %i \n", i, (int)decompressed[i], (int)o_vec[i]);
			pass = false;
		}
	}

	printf("Compressed:\n");
	for (int i=0; i<320; i++) {
		cout << hex << (int)c_vec[i] << " ";
	}
	printf("\nDecompressed:\n");
	for (int i=0; i<320; i++) {
		cout << hex << (int)decompressed[i] << " ";
	}
	printf("\nOriginal\n");
	for (int i=0; i<320; i++) {
		cout << hex << (int)o_vec[i] << " ";
	}
	cout << endl;

	// Output to file
	for (int i=0; i<out_size; i++) {
		d_file << decompressed[i];
	}

	c_file.close();
	o_file.close();
	d_file.close();

	printf("\nFinished Decompression.\n");
	cout << (pass ? "Simulation Passed!" : "Simulation Failed!") << endl;
}
