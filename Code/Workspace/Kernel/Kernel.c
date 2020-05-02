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
#include "Kernel.h"
#define MAXIMUM_PROGRAM_SECTORS_M 26

METHOD_READ_CALCULATOR_T alternateRelativeSectorCalculator = DefaultCalculateRelativeSector;
METHOD_READ_CALCULATOR_T alternateHeadCalculator = DefaultCalculateHead;
METHOD_READ_CALCULATOR_T alternateTrackCalculator = DefaultCalculateTrack;
int useAlternateCalculators = INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_FALSE_M;
int sectorIODeviceNumber = FLOPPY_A_DEVICE_NUMBER_M;
int writeFileStatus = 0;
DIRECTORY_STORAGE_TYPE_M( kernelDirectory );
MAP_STORAGE_TYPE_M( kernelMap );
SECTOR_STORAGE_M( fileSectorStorageBuffer );
char commandLineArgument[ COMMAND_LINE_ARGUMENT_BUFFER_LENGTH_M ];
char commandLineArgumentInitialized = 0;

/*#define KernelhandleInterrupt21errorMessage_DECLARE_M char errorMessage[ 116 ]

KernelhandleInterrupt21errorMessage_DECLARE_M;

#define KernelhandleInterrupt21errorMessage_FILL_CONTENTS_M \
errorMessage[ 0 ] = 'E'; \
errorMessage[ 1 ] = 'r'; \
errorMessage[ 2 ] = 'r'; \
errorMessage[ 3 ] = 'o'; \
errorMessage[ 4 ] = 'r'; \
errorMessage[ 5 ] = ':'; \
errorMessage[ 6 ] = ':'; \
errorMessage[ 7 ] = 'K'; \
errorMessage[ 8 ] = 'e'; \
errorMessage[ 9 ] = 'r'; \
errorMessage[ 10 ] = 'n'; \
errorMessage[ 11 ] = 'e'; \
errorMessage[ 12 ] = 'l'; \
errorMessage[ 13 ] = ':'; \
errorMessage[ 14 ] = ':'; \
errorMessage[ 15 ] = 'v'; \
errorMessage[ 16 ] = 'o'; \
errorMessage[ 17 ] = 'i'; \
errorMessage[ 18 ] = 'd'; \
errorMessage[ 19 ] = ' '; \
errorMessage[ 20 ] = 'h'; \
errorMessage[ 21 ] = 'a'; \
errorMessage[ 22 ] = 'n'; \
errorMessage[ 23 ] = 'd'; \
errorMessage[ 24 ] = 'l'; \
errorMessage[ 25 ] = 'e'; \
errorMessage[ 26 ] = 'I'; \
errorMessage[ 27 ] = 'n'; \
errorMessage[ 28 ] = 't'; \
errorMessage[ 29 ] = 'e'; \
errorMessage[ 30 ] = 'r'; \
errorMessage[ 31 ] = 'r'; \
errorMessage[ 32 ] = 'u'; \
errorMessage[ 33 ] = 'p'; \
errorMessage[ 34 ] = 't'; \
errorMessage[ 35 ] = '2'; \
errorMessage[ 36 ] = '1'; \
errorMessage[ 37 ] = '('; \
errorMessage[ 38 ] = ' '; \
errorMessage[ 39 ] = 'i'; \
errorMessage[ 40 ] = 'n'; \
errorMessage[ 41 ] = 't'; \
errorMessage[ 42 ] = ','; \
errorMessage[ 43 ] = ' '; \
errorMessage[ 44 ] = 'i'; \
errorMessage[ 45 ] = 'n'; \
errorMessage[ 46 ] = 't'; \
errorMessage[ 47 ] = ','; \
errorMessage[ 48 ] = ' '; \
errorMessage[ 49 ] = 'i'; \
errorMessage[ 50 ] = 'n'; \
errorMessage[ 51 ] = 't'; \
errorMessage[ 52 ] = ','; \
errorMessage[ 53 ] = ' '; \
errorMessage[ 54 ] = 'i'; \
errorMessage[ 55 ] = 'n'; \
errorMessage[ 56 ] = 't'; \
errorMessage[ 57 ] = ' '; \
errorMessage[ 58 ] = ')'; \
errorMessage[ 59 ] = ':'; \
errorMessage[ 60 ] = ' '; \
errorMessage[ 61 ] = 'F'; \
errorMessage[ 62 ] = 'u'; \
errorMessage[ 63 ] = 'n'; \
errorMessage[ 64 ] = 'c'; \
errorMessage[ 65 ] = 't'; \
errorMessage[ 66 ] = 'i'; \
errorMessage[ 67 ] = 'o'; \
errorMessage[ 68 ] = 'n'; \
errorMessage[ 69 ] = ' '; \
errorMessage[ 70 ] = 'r'; \
errorMessage[ 71 ] = 'e'; \
errorMessage[ 72 ] = 'q'; \
errorMessage[ 73 ] = 'u'; \
errorMessage[ 74 ] = 'e'; \
errorMessage[ 75 ] = 's'; \
errorMessage[ 76 ] = 't'; \
errorMessage[ 77 ] = 'e'; \
errorMessage[ 78 ] = 'd'; \
errorMessage[ 79 ] = ' '; \
errorMessage[ 80 ] = 't'; \
errorMessage[ 81 ] = 'h'; \
errorMessage[ 82 ] = 'r'; \
errorMessage[ 83 ] = 'o'; \
errorMessage[ 84 ] = 'u'; \
errorMessage[ 85 ] = 'g'; \
errorMessage[ 86 ] = 'h'; \
errorMessage[ 87 ] = ' '; \
errorMessage[ 88 ] = 'A'; \
errorMessage[ 89 ] = 'X'; \
errorMessage[ 90 ] = ' '; \
errorMessage[ 91 ] = 'r'; \
errorMessage[ 92 ] = 'e'; \
errorMessage[ 93 ] = 'g'; \
errorMessage[ 94 ] = 'i'; \
errorMessage[ 95 ] = 's'; \
errorMessage[ 96 ] = 't'; \
errorMessage[ 97 ] = 'e'; \
errorMessage[ 98 ] = 'r'; \
errorMessage[ 99 ] = ' '; \
errorMessage[ 100 ] = 'd'; \
errorMessage[ 101 ] = 'o'; \
errorMessage[ 102 ] = 'e'; \
errorMessage[ 103 ] = 's'; \
errorMessage[ 104 ] = ' '; \
errorMessage[ 105 ] = 'n'; \
errorMessage[ 106 ] = 'o'; \
errorMessage[ 107 ] = 't'; \
errorMessage[ 108 ] = ' '; \
errorMessage[ 109 ] = 'e'; \
errorMessage[ 110 ] = 'x'; \
errorMessage[ 111 ] = 'i'; \
errorMessage[ 112 ] = 's'; \
errorMessage[ 113 ] = 't'; \
errorMessage[ 114 ] = '.'; \
errorMessage[ 115 ] = '\0';*/

char* errorMessage = "derp";


void handleInterrupt21( int AX, int BX, int CX, int DX )
{
	char cmdbuf[ 7 ];
	cmdbuf[ 0 ] = 'c';
	cmdbuf[ 1 ] = 'm';
	cmdbuf[ 2 ] = 'd';
	cmdbuf[ 3 ] = 'b';
	cmdbuf[ 4 ] = 'u';
	cmdbuf[ 5 ] = 'f';
	cmdbuf[ 6 ] = '\0';
	switch( AX )
	{
		case INTERRUPT_21_PRINT_STRING_M:
		{
			switch( CX )
			{
				case INTERRUPT_21_PRINT_STRING_PRINT_LINE_M: {
					PrintLine( ( char* ) BX );
					break;
				}
				case INTERRUPT_21_PRINT_STRING_TERMINAL_PRINT_M: {
					TerminalPrint( ( char* ) BX );
					break;
				}
				case INTERRUPT_21_PRINT_STRING_PRINT_RANGE_M: {
					PrintRange( ( char* ) BX, DX );
					break;
				}
				case INTERRUPT_21_PRINT_STRING_PRINT_RANGE_LINE_M: {
					PrintRangeLine( ( char* ) BX, DX );
					break;
				}
				default: {
					PrintString( ( char* ) BX );
					break;
				}
			}
			break;
		}
		case INTERRUPT_21_READ_STRING_FROM_KEYBOARD_M:
		{
			switch( CX )
			{
				case INTERRUPT_21_READ_STRING_FROM_KEYBOARD_READ_UNLIMITED: {
					RangedReadLineFromKeyboard( ( char* ) BX, ( -1 ) );
					break;
				}
				case INTERRUPT_21_READ_STRING_FROM_KEYBOARD_READ_RANGE_M: {
					RangedReadLineFromKeyboard( ( char* ) BX, DX );
					break;
				}
				default: {
					ReadLineFromKeyboard( ( char* ) BX );
					break;
				}
			}
			break;
		}
		case INTERRUPT_21_READ_SECTOR_M:
		{
			if( useAlternateCalculators == INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_TRUE_M )
				ReadSectorChooseMethodToRead( BX, CX, DX, sectorIODeviceNumber, alternateRelativeSectorCalculator, alternateHeadCalculator, alternateTrackCalculator );
			else
				DefaultReadSector( BX, CX, DX, sectorIODeviceNumber );
			break;
		}
		case INTERRUPT_21_WRITE_SECTOR_M:
		{
			if( useAlternateCalculators == INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_TRUE_M )
				WriteSectorChooseMethodToWrite( BX, CX, DX, sectorIODeviceNumber, alternateRelativeSectorCalculator, alternateHeadCalculator, alternateTrackCalculator );
			else
				DefaultWriteSector( BX, CX, DX, sectorIODeviceNumber );
			break;
		}
		case INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_M: {
			useAlternateCalculators = BX;
			break;
		}
		case INTERRUPT_21_SET_SECTOR_IO_DEVICE_NUMBER: {
			sectorIODeviceNumber = BX;
			break;
		}
		case INTERRUPT_21_SET_ALTERNATE_SECTOR_CHS_M:
		{
			if( BX != INTERRUPT_21_SECTOR_DEFAULT_CALCULATE_RELATIVE_SECTOR_M )
				alternateRelativeSectorCalculator = BX;
			if( CX != INTERRUPT_21_SECTOR_DEFAULT_CALCULATE_HEAD_M )
				alternateHeadCalculator = CX;
			if( DX != INTERRUPT_21_SECTOR_DEFAULT_CALCULATE_TRACK_M )
				alternateTrackCalculator = DX;
			break;
		}
		case INTERRUPT_21_READ_FILE_M: {
			*( ( int* ) DX ) = ReadFile( ( char* ) BX, ( char* ) CX );
			break;
		}
		case INTERRUPT_21_DELETE_FILE_M: {
			DeleteFile( ( char* ) BX );
			break;
		}
		case INTERRUPT_21_WRITE_FILE_M:
		{
			if( BX == INTERRUPT_21_GET_FILE_WRITE_STATUS )
				*( ( int* ) CX ) = writeFileStatus;
			else
				writeFileStatus = WriteFile( ( char* ) BX, ( char* ) CX, DX );
			break;
		}
		case INTERRUPT_21_OBTAIN_FILE_SECTORS_M:
		{
			ReadDirectory( kernelDirectory );
			*( ( unsigned int* ) DX ) = ObtainSectors( ( char* ) BX, kernelDirectory, ( SECTOR_STORAGE_POINTER_T ) CX );
			break;
		}
		case INTERRUPT_21_READ_DIRECTORY_M:
		{
			//ReadDirectory( ( DIRECTORY_POINTER_T ) BX );
			ReadDirectory( kernelDirectory );
			for( CX = 0; CX < SECTOR_SIZE_M; ++CX )
				( ( DIRECTORY_POINTER_T ) BX )[ CX ] = kernelDirectory[ CX ];
			break;
		}
		case INTERRUPT_21_READ_MAP_M:
		{
			//ReadMap( ( MAP_POINTER_T ) BX );
			ReadMap( kernelMap );
			for( CX = 0; CX < SECTOR_SIZE_M; ++CX )
				( ( MAP_POINTER_T ) BX )[ CX ] = kernelMap[ CX ];

			break;
		}
		case INTERRUPT_21_GET_FILE_ENTRY_M: {
			ReadDirectory( kernelDirectory );
			*( ( int* ) CX ) = GetFileEntry( ( char* ) BX, kernelDirectory );
			break;
		}
		case INTERRUPT_21_GET_SECTOR_IO_DEVICE_NUMBER: {
			*( ( int* ) BX ) = sectorIODeviceNumber;
			break;
		}
		case INTERRUPT_21_RUN_PROCESS_M: {
			RunProcess( ( char* ) BX, ( char* ) CX, MAXIMUM_PROGRAM_SECTORS_M, DX );
			break;
		}
		case INTERRUPT_21_EXECUTE_PROGRAM: {
			ExecuteProgram( ( char* ) BX, CX );
			break;
		}
		case INTERRUPT_21_PROCESSES_RUNNING_M: {
			*( ( unsigned int* ) BX ) = processesRunning;
			break;
		}
		case INTERRUPT_21_TERMINATE_M: {
			Terminate();
			break;
		}
		case INTERRUPT_21_PRINT_AT_LOCATION_M: {
			PrintAtLocation( BX, CX, DX );
			break;
		}
		case INTERRUPT_21_SET_CURSOR_LOCATION_M: {
			SetCursorLocation( BX, CX );
			break;
		}
		case INTERRUPT_21_READ_CHARECHTER_FROM_KEYBOARD_M: {
			*( ( char* ) BX ) = interrupt( 0x16, 0, 0, 0, 0 );
			break;
		}
		case INTERRUPT_21_CONTROL_KEYS_STATUS_M: {
			*( ( int* ) BX ) = interrupt( 0x16, WRITE_PARAMETER_M( 0, 0x02 ), 0, 0, 0 );
			break;
		}
		case INTERRUPT_21_COMMAND_LINE_ARGUMENT_GET_M:
		{
			if( commandLineArgumentInitialized == 0 )
			{
				for( DX = 0; DX < COMMAND_LINE_ARGUMENT_BUFFER_LENGTH_M; ++DX )
					commandLineArgument[ DX ] = '\0';
				commandLineArgumentInitialized = 1;
			}
			ReadFile( cmdbuf, commandLineArgument );
			for( DX = 0; DX < COMMAND_LINE_ARGUMENT_BUFFER_LENGTH_M && commandLineArgument[ DX ] != '\0'; ++DX )
				( ( char* ) BX )[ DX ] = commandLineArgument[ DX ];
			break;
		}
		case INTERRUPT_21_COMMAND_LINE_ARGUMENT_SET_M:
		{
			if( commandLineArgumentInitialized == 0 )
			{
				for( DX = 0; DX < COMMAND_LINE_ARGUMENT_BUFFER_LENGTH_M; ++DX )
					commandLineArgument[ DX ] = '\0';
				commandLineArgumentInitialized = 1;
			}
			for( AX = 0; AX < CX && ( ( char* ) BX )[ AX ] != '\0'; ++AX )
				commandLineArgument[ AX ] = ( ( char* ) BX )[ AX ];
			WriteFile( cmdbuf, commandLineArgument, 1 );
			break;
		}
		default: {
			/*KernelhandleInterrupt21errorMessage_FILL_CONTENTS_M*/
			PrintLine( errorMessage );
			break;
		}
	}
}
