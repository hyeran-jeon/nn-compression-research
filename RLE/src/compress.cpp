#include "compress.hpp"

void compress(fstream &ins, fstream &outs)
{
	int in_size  	= 0;
	int out_size 	= 0;
	uchar character = ins.get();	// first char
	int count 	 	= 1;

	while (!ins.eof())
	{
		uchar c = ins.get();

		// Discard EOF character
		if (ins.eof()) break;

		in_size++;

		if (c == character) count++;

		else
		{
			replace(character, count, out_size, outs);
			character = c;
			count = 1;
		}
	}

	// Handle remaining sequence
	replace(character, count, out_size, outs);

	float percentage = (1 - ((float)out_size/in_size)) * 100;

	// Print information
	cout << "-------------------------------------------------" 	 << endl;
	cout << setw(12) << left << "Uncompressed Size : " 	<< in_size	 << endl;
	cout << setw(12) << left << "Compressed Size   : " 	<< out_size  << endl;
	cout << setw(12) << left << "Percent Compressed: "
		 			 << fixed << setprecision(1)  << percentage << "%" << endl;
}


void replace(uchar character, int &count, int &out_size, fstream &outs)
{
	// Check if the character is the same as the escape character
	// If it is, this character needs to escape itself
	bool escape_escape = (character == ESCAPE);

	// Decrease count to a single digit
	while (count > 9)
	{
		if (escape_escape) {
			outs << ESCAPE;
			out_size++;
		}
		outs << ESCAPE << (uchar)character << (uchar)'9';
		count 	 -= 9;
		out_size += 3;
	}
	// If count is over 3 characters, then compress
	if (count > 3)
	{
		if (escape_escape) {
			outs << ESCAPE;
			out_size++;
		}
		outs << ESCAPE << (uchar)character << (uchar)count;
		out_size += 3;
	}
	// If count is 1-3
	else
	{
		// If character is the escape character, need to compress
		if (character == ESCAPE)
		{
			outs << ESCAPE << ESCAPE << (uchar)count;
			out_size += 3;
		}
		// Else don't compress
		else
		{
			for (int i=0; i<count; i++) {
				outs << (uchar)character;
				out_size++;
			}
		}
	}
}
