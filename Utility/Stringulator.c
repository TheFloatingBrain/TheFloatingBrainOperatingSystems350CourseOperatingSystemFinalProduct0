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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int SKIP_ARGUMENTS_C = 1;
#define FILE_NAME_M 0
#define FUNCTION_NAME_M 1
#define STRING_NAME_M 2

char* fileName = 0;
char* functionName = 0;
char* stringName = 0;
char* stringContents = 0;

int main( int argc, char** args )
{
    int i;
    if( ( argc - SKIP_ARGUMENTS_C ) < 4 )
    {
        printf( "Error: Not enough arguments supplied, should be: file name "
                "containing string *space* function name *space* string name "
                "*space* string contents... " );
        return 1;
    }
    stringContents = malloc( 2 * sizeof( char ) );
    stringContents[ 0 ] = '\0';
    stringContents[ 1 ] = '\0';
    for( i = SKIP_ARGUMENTS_C; i < argc; ++i )
    {
        switch( i - SKIP_ARGUMENTS_C )
        {
            case FILE_NAME_M: {
                fileName = args[ i ];
                break;
            }
            case FUNCTION_NAME_M: {
                functionName = args[ i ];
                break;
            }
            case STRING_NAME_M: {
                stringName = args[ i ];
                break;
            }
            default: 
            {
                char* oldStringContents = stringContents;
                stringContents = malloc( strlen( stringContents ) + strlen( args[ i ] ) + 2 );
                strcpy( stringContents, oldStringContents );
                stringContents[ strlen( oldStringContents ) ] = ' ';
                strcpy( &stringContents[ strlen( oldStringContents ) + 1 ], args[ i ] );
                free( oldStringContents );
                break;
            }
        }
    }
    const int STRING_CONTENTS_LENGTH_C = strlen( stringContents );
    printf( "#define %s%s%s_DECLARE_M char %s[ %d ];\n\n", fileName, functionName, stringName, stringName, STRING_CONTENTS_LENGTH_C );
    printf( "#define %s%s%s_FILL_CONTENTS_M \\\n", fileName, functionName, stringName );
    for( i = 1; i < STRING_CONTENTS_LENGTH_C; ++i )
        printf( "%s[ %d ] = '%c'; \\\n", stringName, i - 1, stringContents[ i ] );
    printf( "%s[ %d ] = '\\0';\n", stringName, i - 1 );
    return 0;
}
