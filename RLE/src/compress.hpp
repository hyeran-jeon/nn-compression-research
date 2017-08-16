#pragma once

#include <iostream>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

#define ESCAPE ((unsigned char)0x01)

typedef unsigned char uchar;

void compress(fstream &ins, fstream &outs);
void replace(uchar character, int &count, int &out_size, fstream &outs);
