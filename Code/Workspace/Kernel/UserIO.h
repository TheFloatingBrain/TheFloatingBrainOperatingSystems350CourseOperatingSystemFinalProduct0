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
#include "SectorIO.h"

#ifndef USERIO_H
	#define USERIO_H
	/*PrintCharToVRAM Stuff*/
	#define VRAM_BEGIN_M 160
	#define BYTES_PER_CHARECHTER_M 2
	#define MACRO_VRAM_BEGIN_M 0xB800
	#define CYCLE_TEXT_COLOR_M( CURRENT_COLOR ) ( ( CURRENT_COLOR > 14 ) ? CURRENT_COLOR = 0 : ++CURRENT_COLOR )
	/*PrintString Stuff*/
	#define BIOS_PRINT_M 0x10
	#define BIOS_PRINT_CHAR_M 0xE
	/*ReadLineFromKeyboard Stuff.*/
	#define BIOS_READ_KEYBOARD_LINE_M 0x16
	#define ENTER_KEY_M 0xD
	#define LINE_FEED_M 0xA
	#define BACKSPACE_KEY_M 0x8
	#define READ_LINE_FROM_KEYBOARD_UNLIMITED_LENGTH_M -1
	#define READ_LINE_FROM_KEYBOARD_MINIMUM_LENGTH_M 80

	void PrintCharToVRAM( char toPrint, unsigned short offset, char color );
	void InfiniteCharPrintHello();

	void PrintString( char* toPrint );
	void PrintLine( char* toPrint );
	void TerminalPrint( char* toPrint );
	void PrintRange( char* toPrint, unsigned int length );
	void PrintRangeLine( char* toPrint, unsigned int length );

	/*TODO: Change to char* ReadLineFromKeyboard() using dynamic memory allocation*/
	void RangedReadLineFromKeyboard( char* buffer, int length );
	void ReadLineFromKeyboard( char* buffer );

	void PrintAtLocation( unsigned int x, unsigned int y, char toPrint );
	void SetCursorLocation( unsigned int x, unsigned int y );
	/*void GetCursorLocation( unsigned int* x, unsigned int* y );*/
#endif
