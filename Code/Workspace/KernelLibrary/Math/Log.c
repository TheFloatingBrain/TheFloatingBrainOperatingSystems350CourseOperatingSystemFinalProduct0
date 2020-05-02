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
#include "Log.h"

unsigned int ToPositivePower( unsigned int base, unsigned int power )
{
	unsigned int i = 1;
	unsigned int buffer = base;
	if( power <= 0 )
		return 1;
	for( ; i < power; ++i )
		buffer *= base;
	return buffer;
}

int Log10( int number )
{
	unsigned int tenBuffer = 1;
	unsigned int bufferCount = 0;
	unsigned int isLess = 0;
	/*|number|*/
	number = number < 0 ? -number : number;
	if( number < 10 )
		return 0;
	/*Maximum a 16 bit integer can hold is 65...*/
	while( 1 )
	{
		isLess = Divide( number, tenBuffer ) < 10 ? 1 : 0;
		if( tenBuffer >= 10000 && isLess == 0 )
			return LOG_10_LOG_NOT_FOUND_M;
		else if( isLess == 0 ) {
			tenBuffer *= 10;
			++bufferCount;
		}
		else
			return bufferCount;
	}
}

int RangedBase10HexASCIIStringToInteger( char* toInteger, unsigned int begin, unsigned int end )
{
	int i = begin;
	int buffer = 0;
	for( ; i < end; ++i ) {
		buffer += ( ( ( unsigned int ) toInteger[ i ] ) * 
				ToPositivePower( 16, i - begin ) );
	}
	return buffer;
}

void NumberToString( char* buffer, int number )
{
	int copy = number < 0 ? -number : number;
	int power = 0;
	int digit = 0;
	int maxPower;
	int raised;
	maxPower = Log10( number );
	power = maxPower;
	for( ; power >= 0; --power )
	{
		raised = ToPositivePower( 10, power );
		digit = Divide( copy, raised );
		buffer[ maxPower - power ] = ( char ) ( digit + 48 );
		copy -= digit * raised;
	}
}

