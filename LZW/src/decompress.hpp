#pragma once
#include "globals.hpp"
#include "stack.hpp"

void decompress(STREAM &in, STREAM &out, int in_size, int &out_size);

void bram_decompress(uchar in_file[120000], uchar out_file[150000], int in_size, int &out_size);
