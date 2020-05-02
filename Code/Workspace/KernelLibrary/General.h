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
#ifndef GENERAL_H_HEADER_INCLUDED_M
#define GENERAL_H_HEADER_INCLUDED_M

/*General Stuff*/
#define PARAMETER_SIZE_M 256
#define WRITE_PARAMETER_M( WHAT, INSTRUCTION ) INSTRUCTION * PARAMETER_SIZE_M + WHAT
/*<StringConstants>*/
	#define NULL_TERMINATOR_M '\0'
	#define NEW_LINE_M '\n'
	#define CARRIAGE_RETURN_M '\r'
/*</StringConstants>*/

#define SECTOR_SIZE_M 512
#define FLOPPY_A_DEVICE_NUMBER_M 0


#define COMMAND_LINE_ARGUMENT_BUFFER_LENGTH_M 64

#define TEXT_EDITOR_COMMAND_LINE_MODE_M "txtedtcmd"
#define TEXT_EDITOR_COMMAND_LINE_MODE_LENGTH_M 9

/*Hopes of dynamic and larger file sizes, abstract away the limits.*/
#define SECTOR_STORAGE_M( nameOfSectorStorer ) unsigned char nameOfSectorStorer [ ENTRY_LENGTH_M - FILE_NAME_SIZE_LIMIT_M ]
typedef unsigned char* SECTOR_STORAGE_POINTER_T;
#define DIRECTORY_STORAGE_TYPE_M( name ) char name [ SECTOR_SIZE_M ]
#define MAP_STORAGE_TYPE_M( name ) char name [ SECTOR_SIZE_M ]
typedef char* DIRECTORY_POINTER_T;
typedef char* MAP_POINTER_T;

/*<FileStuff>*/
#define ENTRY_LENGTH_M 32
#define FILE_NAME_SIZE_LIMIT_M 6
/*</FileStuff>*/

/*<Interrupts>*/
	/*<Sector>*/
		/*<ConsolePrint>*/
			#define INTERRUPT_21_PRINT_STRING_M 0
			/*<Options>*/
				#define INTERRUPT_21_PRINT_STRING_PRINT_STRING_M 0
				#define INTERRUPT_21_PRINT_STRING_PRINT_LINE_M 1
				#define INTERRUPT_21_PRINT_STRING_TERMINAL_PRINT_M 2
				#define INTERRUPT_21_PRINT_STRING_PRINT_RANGE_M 3
				#define INTERRUPT_21_PRINT_STRING_PRINT_RANGE_LINE_M 4
			/*</Options>*/
			#define INTERRUPT_21_PRINT_AT_LOCATION_M 167
			#define INTERRUPT_21_SET_CURSOR_LOCATION_M 168
		/*</ConsolePrint>*/
		/*<ReadKeyboard>*/
			#define INTERRUPT_21_READ_STRING_FROM_KEYBOARD_M 1
			/*<Options>*/
				#define INTERRUPT_21_READ_STRING_FROM_KEYBOARD_READ_DEFAULT_M 0
				#define INTERRUPT_21_READ_STRING_FROM_KEYBOARD_READ_UNLIMITED 1
				#define INTERRUPT_21_READ_STRING_FROM_KEYBOARD_READ_RANGE_M 2
			/*</Options>*/
			#define INTERRUPT_21_CONTROL_KEYS_STATUS_M 169
			/*TODO: Change later to just invoke interrupt 16?*/
			#define INTERRUPT_21_READ_CHARECHTER_FROM_KEYBOARD_M 170
		/*</ReadKeyboard>*/
	/*</Sector>*/
	/*<Buffers>*/
		#define INTERRUPT_21_COMMAND_LINE_ARGUMENT_SET_M 171
		#define INTERRUPT_21_COMMAND_LINE_ARGUMENT_GET_M 172
	/*</Buffers>*/
	/*<SectorIO>*/
		/*<ReadSector>*/
			#define INTERRUPT_21_READ_SECTOR_M 2
		/*</ReadSector>*/
		/*<WriteSector>*/
			#define INTERRUPT_21_WRITE_SECTOR_M 6
		/*</WriteSector>*/
		/*<CHS>*/
			#define INTERRUPT_21_SECTOR_DEFAULT_CALCULATE_RELATIVE_SECTOR_M 0
			#define INTERRUPT_21_SECTOR_DEFAULT_CALCULATE_HEAD_M 0
			#define INTERRUPT_21_SECTOR_DEFAULT_CALCULATE_TRACK_M 0
			#define INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_M 27
			#define INTERRUPT_21_SET_ALTERNATE_SECTOR_CHS_M 10
			#define INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_TRUE_M 2
			#define INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_FALSE_M 1
		/*</CHS>*/
		/*<Device>*/
			#define INTERRUPT_21_SET_SECTOR_IO_DEVICE_NUMBER 9
			#define INTERRUPT_21_GET_SECTOR_IO_DEVICE_NUMBER 11
		/*</Device>*/
	/*</SectorIO>*/
	/*<FileIO>*/
		#define INTERRUPT_21_READ_FILE_M 3
		#define INTERRUPT_21_DELETE_FILE_M 7
		#define INTERRUPT_21_WRITE_FILE_M 8
		#define INTERRUPT_21_OBTAIN_FILE_SECTORS_M 37
		#define INTERRUPT_21_READ_DIRECTORY_M 38
		#define INTERRUPT_21_READ_MAP_M 39
		#define INTERRUPT_21_GET_FILE_ENTRY_M 40
		/*<Options>*/
			#define INTERRUPT_21_GET_FILE_WRITE_STATUS 0
		/*</Options>*/
	/*</FileIO>*/
	/*<ProcessAPI>*/
		#define INTERRUPT_21_RUN_PROCESS_M 28
		#define INTERRUPT_21_EXECUTE_PROGRAM 4
		#define INTERRUPT_21_TERMINATE_M 5
		#define INTERRUPT_21_PROCESSES_RUNNING_M 29
	/*</ProccessAPI>*
/*</Interrupts>*/
#endif
