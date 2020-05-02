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
#include "Process.h"

#define MAXIMUM_PROGRAM_SECTORS_M 26

unsigned int processesRunning = 0;

int RunProcess( char* processName, char* processMemory, int processLengthInSectors, unsigned int segment )
{
    unsigned int processMemoryBottem = 0x0000;
    unsigned int i = 0;
 //   char dbg[ 3 ];
    ++processesRunning;
   // dbg[ 0 ] = '$';
  //  dbg[ 1 ] = '$';
    //dbg[ 2 ] = '\0';
    //dbg[ 1 ] = 48 + processesRunning;
    //interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "MEMES: ", 0, 0 );
    //interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, dbg, INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
    for( ; i < processLengthInSectors * SECTOR_SIZE_M; ++i )
    {
        putInMemory( segment, processMemoryBottem, processMemory[ i ] );
        ++processMemoryBottem;
    }
    launchProgram( segment );
    return 0;
}
int ExecuteProgram( char* programName, unsigned int segment )
{
    char programData[ SECTOR_SIZE_M * MAXIMUM_PROGRAM_SECTORS_M ];
    unsigned int success = ReadFile( programName, programData );
    unsigned int processMemoryBottem = 0x0000;
    if( success == READ_FILE_DID_NOT_FIND_FILE_M ) {
        PrintLine( "Error::StarProgram( char*, unsigned int ): Failed to read program file." );
        return FAILED_TO_READ_PROGRAM_M;
    }
    return RunProcess( programName, programData, success, segment );
}

void Terminate()
{
    char shell[ 6 ];
	shell[ 0 ] = 'S';
	shell[ 1 ] = 'h';
	shell[ 2 ] = 'e';
	shell[ 3 ] = 'l';
	shell[ 4 ] = 'l';
	shell[ 5 ] = '\0';
    --processesRunning;
    ExecuteProgram( shell, 0x2000 );
}
