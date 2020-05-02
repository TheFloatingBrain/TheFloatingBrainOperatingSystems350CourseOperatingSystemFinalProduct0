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
#include "Find.h"

unsigned int FindStringLength( char* toDetermenLength )
{
    unsigned int i = 0;
    for( ; toDetermenLength[ i ] != NULL_TERMINATOR_M; ++i );
    return ++i;
}
int RangedFindNextOf( char* set, char* query, unsigned int begin, unsigned int end, unsigned int queryLength )
{
    unsigned int i = begin;
    unsigned int j = 0;
    unsigned int k = i;
    unsigned short found = 1;
    for( ; i < end; ++i )
    {
        k = i;
        found = 1;
        for( j = 0; j < queryLength; ++j )
        {
            if( k >= end )
                found = 0;
            if( set[ k ] != query[ j ] ) {
                found = 0;
                break;
            }
            ++k;
        }
        if( found == 1 )
            return i;
    }
    return NEXT_NOT_FOUND_M;
}
int RangedFindNextWhiteSpace( char* set, unsigned int begin, unsigned int end )
{
    unsigned int found[ 7 ];
    unsigned int shortest = NEXT_NOT_FOUND_M;
    unsigned int i = 0;
    char* whiteSpaces = " \f\n\r\t\v\0";
    found[ 0 ] = RangedFindNextOf( set, whiteSpaces, begin, end, 1 );
    found[ 1 ] = RangedFindNextOf( set, ++whiteSpaces, begin, end, 1 );
    found[ 2 ] = RangedFindNextOf( set, ++whiteSpaces, begin, end, 1 );
    found[ 3 ] = RangedFindNextOf( set, ++whiteSpaces, begin, end, 1 );
    found[ 4 ] = RangedFindNextOf( set, ++whiteSpaces, begin, end, 1 );
    found[ 5 ] = RangedFindNextOf( set, ++whiteSpaces, begin, end, 1 );
    found[ 6 ] = RangedFindNextOf( set, ++whiteSpaces, begin, end, 1 );
    for( ; i < 7 + 1; ++i )
        shortest = ( ( found[ i ] < shortest ) ? found[ i ] : shortest );
    /*interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "The whitespace is ", INTERRUPT_21_PRINT_STRING_PRINT_STRING_M, 0 );
    if( shortest != NEXT_NOT_FOUND_M )
    {
        switch( shortest )
        {
            case ' ': {
                interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "space", INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
                break;
            }
            case '\f': {
                interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "feed", INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
                break;
            }
            case '\n': {
                interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "new line", INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
                break;
            }
            case '\r': {
                interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "return", INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
                break;
            }
            case '\t': {
                interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "tab", INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
                break;
            }
            case '\v': {
                interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "vertical tab", INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
                break;
            }
            case '\0': {
                interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "null terminator", INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
                break;
            }
            default:
                break;
        }
    }*/
    return shortest;
}
int FindNextOf( char* set, char* query ) {
    return RangedFindNextOf( set, query, 0, FindStringLength( set ), FindStringLength( query ) );
}
int FindNextOfChar( char* set, char query ) {
    return RangedFindNextOf( set, query, 0, FindStringLength( set ), 1 );
}
int FindNextWhiteSpace( char* set ) {
    return RangedFindNextWhiteSpace( set, 0, FindStringLength( set ) );
}
