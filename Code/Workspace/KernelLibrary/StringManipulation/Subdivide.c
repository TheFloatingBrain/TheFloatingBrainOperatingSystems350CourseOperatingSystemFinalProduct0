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
#include "Subdivide.h"

#define IS_WHITE_SPACE_M 1
#define IS_NOT_WHITE_SPACE_M 0

unsigned char IsWhiteSpace( char query ) {
    return !( query > 32 && query <= 126 );
}

void BufferedSplitRanged( char** buffer, char* what, char* by, unsigned int begin, unsigned int end, unsigned int byLength )
{
    int i = begin;
    int j = 0;
    int last = i;
    int subStrings = 0;
    int toAdd = 0;
    while( i < end )
    {
        i = RangedFindNextOf( what, by, i + 1, end, byLength );
        if( i == NEXT_NOT_FOUND_M )
            break;
        toAdd = ( ( subStrings == 0 ) ? 0 : byLength );
        for( j = ( last + toAdd ); j < i; ++j )
            buffer[ subStrings ][ ( j - last ) ] = what[ j ];
        buffer[ subStrings ][ ( j - ( last + toAdd ) ) ] = '\0';
        ++subStrings;
    }
}

void BufferedSplitByWhiteSpaceRanged( char* buffer, char* what, unsigned int width, unsigned int begin, unsigned int end )
{
    unsigned int i = 0;
    unsigned int last = 0;
    unsigned int j = 0;
    unsigned int subStrings = 0;
    char proceed = 0;
    while( i < end )
    {
        if( !IsWhiteSpace( what[ i ] ) )
            break;
        ++i;
    }
    last = i;
    while( i < end )
    {
        proceed = 0;
        i = RangedFindNextWhiteSpace( what, i, end );
        proceed = ( i == 1 ) ? !IsWhiteSpace( what[ 0 ] ) : ( ( i - last > 1 ) ? 1 : 
                ( ( ( int ) ( i - 2 ) > 0 ) ? ( IsWhiteSpace( what[ i ] ) && 
                !IsWhiteSpace( what[ i - 1 ] ) && IsWhiteSpace( what[ i - 2 ] ) ) : 0 ) );
        if( proceed == 1 )
        {
            for( j = last; j < i; ++j )
                buffer[ ( subStrings * width ) + ( j - last ) ] = what[ j ];
            buffer[ ( subStrings * width ) + ( j - last ) ] = '\0';
            ++subStrings;
        }
        if( i == NEXT_NOT_FOUND_M || i < 1 )
        	return;
        last = i + 1;
        ++i;
    }
}
