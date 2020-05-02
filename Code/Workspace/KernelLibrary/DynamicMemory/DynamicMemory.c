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
#include "DynamicMemory.h"

#include "../Math/Log.h"

unsigned int* dynamicMemory;
size_t dynamicMemorySize;

void MemorySet( void* memory, int to, size_t amountOfMemory )
{
	int i = 0;
	unsigned int* memoryUnsignedInt = ( unsigned int* ) memory;
	for( ; i < amountOfMemory; ++i )
		memoryUnsignedInt[ i ] = ( unsigned int ) to;
}


void DynamicMemoryStart( unsigned int* dynamicMemory_, unsigned int dynamicMemorySize_ )
{
    dynamicMemory = dynamicMemory_;
    dynamicMemorySize = dynamicMemorySize_;
    MemorySet( dynamicMemory, BLOCK_NOT_IN_USE_M, dynamicMemorySize );
}

/*Quick and dirty malloc.*************
Lord forgive me this implementation.*/
void* DynamicAllocate( size_t size )
{
	int i = 0;
	int blockSizeIndex = i;
	int state = READING_BLOCK_IN_USE_M;
	//////////////////////////////////////////////
	char dbg[ 5 ];
	//////////////////////////////////////////////
	for( ; i < dynamicMemorySize; ++i )
	{
		if( state == READING_BLOCK_IN_USE_M )
		{
			if( dynamicMemory[ i ] == BLOCK_NOT_IN_USE_M ) {
				blockSizeIndex = i;
				state = READING_BLOCK_SIZE_M;
//				MemorySet( dbg, '$', 4 );
//				dbg[ 4 ] = '\0';
//				NumberToString( dbg, i );
//				interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "Reading: ", INTERRUPT_21_PRINT_STRING_PRINT_STRING_M, 0, 0 );
//				interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, dbg, INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
			}
			else if( ( i + 1 ) < dynamicMemorySize )
				i += dynamicMemory[ i + 1 ];
			else
				break;
		}
		if( state == READING_BLOCK_SIZE_M && ( blockSizeIndex + 1 ) <= dynamicMemorySize )
		{
			if( dynamicMemory[ ( blockSizeIndex + 1 ) ] >= size || dynamicMemory[ ( blockSizeIndex + 1 ) ] == 0 )
				state = WRITING_BLOCK_M;
			else
			{
				i = ( blockSizeIndex + dynamicMemory[ blockSizeIndex + 1 ] );
				state = READING_BLOCK_IN_USE_M;
//				break;
			}
		}
		if( state == WRITING_BLOCK_M )// && ( blockSizeIndex + 2 + size ) < dynamicMemorySize )
		{
			dynamicMemory[ blockSizeIndex ] = BLOCK_IN_USE_M;
			dynamicMemory[ blockSizeIndex + 1 ] = size;
			return ( ( void* ) &dynamicMemory[ blockSizeIndex + 2 ] );
		}
	}
	interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "Error::KernelCSupportLibrary::DynamicAllocate( size_t ): out of dynamic memory!", INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
	return 0;
}

void Deallocate( void* toDeallocate )
{
	unsigned int* toDeallocateUnsignedInt = ( ( unsigned int* ) toDeallocate );
	--toDeallocateUnsignedInt;
	--toDeallocateUnsignedInt;
	*toDeallocateUnsignedInt = BLOCK_NOT_IN_USE_M;
}
