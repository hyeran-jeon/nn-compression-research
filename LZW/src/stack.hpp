#pragma once
#include "globals.hpp"

#define STACK_DEPTH 100

class stack
{
	public:
		stack();
		int 	push(int byte);
		int  	pop();
		bool 	empty();

	private:
		int 	array[STACK_DEPTH];
		int 	index;
};

inline stack::stack()
{
	for (int i=0; i<STACK_DEPTH; i++) {
#pragma HLS PIPELINE II=1
		array[i] = -1;
	}
	index = 0;
}

inline int stack::push(int byte)
{
	if (index == STACK_DEPTH) return -1;

	array[index++] = byte;
	return 0;
}

inline int stack::pop()
{
	if (index == 0) return -1;
	else 			return array[--index];
}

inline bool stack::empty()
{
	return (index == 0);
}
