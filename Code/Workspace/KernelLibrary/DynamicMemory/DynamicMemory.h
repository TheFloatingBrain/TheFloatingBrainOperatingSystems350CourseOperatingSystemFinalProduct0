/*
Copyright (C) 2019 Christopher A. Greeley

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "../General.h"

#define READING_BLOCK_SIZE_M 0
#define READING_BLOCK_IN_USE_M 1
#define WRITING_BLOCK_M 2
#define BLOCK_IN_USE_M 1
#define BLOCK_NOT_IN_USE_M 0

typedef unsigned int size_t;

extern unsigned int* dynamicMemory;
extern size_t dynamicMemorySize;

void MemorySet( void* memory, int to, size_t amountOfMemory );

void DynamicMemoryStart( unsigned int* dynamicMemory_, unsigned int dynamicMemorySize_ );

void* DynamicAllocate( size_t size );

void Deallocate( void* toFree );
