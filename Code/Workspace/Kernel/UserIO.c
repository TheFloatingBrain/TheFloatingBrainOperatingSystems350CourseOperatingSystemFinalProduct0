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
#include "UserIO.h"

#define DO_NEW_LINE interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( NEW_LINE_M, BIOS_PRINT_CHAR_M ), 0, 0, 0 ); \
		interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( CARRIAGE_RETURN_M, BIOS_PRINT_CHAR_M ), 0, 0, 0 );

void PrintCharToVRAM( char toPrint, unsigned short offset, char color )
{
	putInMemory( MACRO_VRAM_BEGIN_M, VRAM_BEGIN_M + ( BYTES_PER_CHARECHTER_M  * offset ), toPrint );
	putInMemory( MACRO_VRAM_BEGIN_M, ( VRAM_BEGIN_M + ( BYTES_PER_CHARECHTER_M * offset ) ) + 0x001, color );
}

void InfiniteCharPrintHello()
{
	unsigned short i = 0;
	unsigned short color = 0;
	while( 1 )
	{
		PrintCharToVRAM( 'H', 0, CYCLE_TEXT_COLOR_M( color ) );
		PrintCharToVRAM( 'e', 1, CYCLE_TEXT_COLOR_M( color ) );
		PrintCharToVRAM( 'l', 2, CYCLE_TEXT_COLOR_M( color ) );
		PrintCharToVRAM( 'l', 3, CYCLE_TEXT_COLOR_M( color ) );
		PrintCharToVRAM( 'o', 4, CYCLE_TEXT_COLOR_M( color ) );
		for( i = 0; i < 500; ++i );
	}
}

void PrintString( char* toPrint )
{
	unsigned int i = 0;
	while( toPrint[ i ] != NULL_TERMINATOR_M ) {
		interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( toPrint[ i ], BIOS_PRINT_CHAR_M ), 0, 0, 0 );
		++i;
	}
}
void PrintLine( char* toPrint )
{
	PrintString( toPrint );
	DO_NEW_LINE
}
void TerminalPrint( char* toPrint )
{
	unsigned int i = 0;
	DO_NEW_LINE
	while( toPrint[ i ] != NULL_TERMINATOR_M )
	{
		interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( toPrint[ i ], BIOS_PRINT_CHAR_M ), 0, 0, 0 );
		if( toPrint[ i ] == NEW_LINE_M )
			interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( CARRIAGE_RETURN_M, BIOS_PRINT_CHAR_M ), 0, 0, 0 );
		++i;
	}
}

void PrintRange( char* toPrint, unsigned int length )
{
	unsigned int i = 0;
	for( ; i < length; ++i )
		interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( toPrint[ i ], BIOS_PRINT_CHAR_M ), 0, 0, 0 );
}

void PrintRangeLine( char* toPrint, unsigned int length ) {
	PrintRange( toPrint, length );
	DO_NEW_LINE
}

void RangedReadLineFromKeyboard( char* buffer, int length )
{
	unsigned int keysPressed = 0;
	unsigned int furthestKey = keysPressed;
	unsigned int rangeCheck = ( ( length == READ_LINE_FROM_KEYBOARD_UNLIMITED_LENGTH_M ) ? 1 : 0 );
	char currentCharechter;
	char currentPrintOut[ 2 ];
	currentPrintOut[ 1 ] = NULL_TERMINATOR_M;
	if( length != READ_LINE_FROM_KEYBOARD_UNLIMITED_LENGTH_M && length < READ_LINE_FROM_KEYBOARD_MINIMUM_LENGTH_M ) {
		PrintLine( "Error::Kernel::void RangedReadLineFromKeyboard( char*, int ): Length is not unlimited and is less than minimum (80)." );
		return;
	}
	while( 1 )
	{
		currentCharechter = interrupt( BIOS_READ_KEYBOARD_LINE_M, 0, 0, 0, 0 );
		if( currentCharechter == ENTER_KEY_M )
			break;
		if( rangeCheck == 1 )
		{
			/*Leave enough room for line feed and '\0'*/
			if( keysPressed >= ( length - 2 ) )
				break;
		}
		if( currentCharechter == BACKSPACE_KEY_M )
		{
			if( keysPressed > 0 )
			{
				--keysPressed;
				currentPrintOut[ 0 ] = BACKSPACE_KEY_M;
				PrintString( currentPrintOut );
				currentPrintOut[ 0 ] = ' ';
				PrintString( currentPrintOut );
				currentPrintOut[ 0 ] = BACKSPACE_KEY_M;
				PrintString( currentPrintOut );
			}
			else {
				currentPrintOut[ 0 ] = NULL_TERMINATOR_M;
				PrintString( currentPrintOut );
			}
		}
		else
		{
			buffer[ keysPressed ] = currentCharechter;
			++keysPressed;
			currentPrintOut[ 0 ] = currentCharechter;
			PrintString( currentPrintOut );
		}
		furthestKey = keysPressed > furthestKey ? keysPressed : furthestKey;
	}
	DO_NEW_LINE
	while( keysPressed <= furthestKey && ( rangeCheck == 1 ? keysPressed < length - 3 : 1 ) )
		buffer[ keysPressed++ ] = NULL_TERMINATOR_M;
	buffer[ ++keysPressed ] = LINE_FEED_M;
	buffer[ ++keysPressed ] = NULL_TERMINATOR_M;
}

void ReadLineFromKeyboard( char* buffer ) {
	RangedReadLineFromKeyboard( buffer, READ_LINE_FROM_KEYBOARD_UNLIMITED_LENGTH_M );
}

void PrintAtLocation( unsigned int x, unsigned int y, char toPrint )
{
    interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( 0, 0x02 ), 
            0, 0, WRITE_PARAMETER_M( x, y ) );
    interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( toPrint, 0x0A ), 
            0, 1, 0 );
}

void SetCursorLocation( unsigned int x, unsigned int y ) {
	interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( 0, 0x02 ), 
		0, 0, WRITE_PARAMETER_M( x, y ) );
}

/*void GetCursorLocation( unsigned int* x, unsigned int* y )
{
	int cx, dx;
	char coordinate[ 2 ];
	interrupt( BIOS_PRINT_M, WRITE_PARAMETER_M( 0, 0x03 ), 0, &cx, coordinate );
//	coordinate = &dx;
	*x = ( coordinate[ 0 ] );
	*y = ( coordinate[ 2 ] );
//	bx = bh * 256 + bl
//	bh * 256 = bx - bl
}*/
