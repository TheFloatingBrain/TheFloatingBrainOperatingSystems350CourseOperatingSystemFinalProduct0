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
#include "../KernelLibrary/KernelCSupportLibrary.h"

//#ifndef SECTOR_IO_H
	#define SECTOR_IO_H
	/*#define INTERRUPT_21_GET_SECTOR_IO_DEVICE_NUMBER 11*/
	#define SECTOR_IO_INTERRUPT_M 0x13
	/*Read Sector Stuff.*/
	#define SECTOR_IO_READ_FUNCTION_M 2
	int ReadSectorChooseMethodToRead( char* buffer, unsigned int sectorNumber, unsigned int amouontOfSectorsToRead, int deviceNumber, 
			METHOD_READ_CALCULATOR_T calculateRelativeSector, METHOD_READ_CALCULATOR_T calculateHead, METHOD_READ_CALCULATOR_T calculateTrack );
	int DefaultReadSector( char* buffer, unsigned int sectorNumber, unsigned int amouontOfSectorsToRead, int deviceNumber );
	/*Write Sector Stuff.*/
	#define SECTOR_IO_WRITE_FUNCTION_M 3
	int WriteSectorChooseMethodToWrite( char* buffer, unsigned int sectorNumber, unsigned int amouontOfSectorsToRead, int deviceNumber, 
			METHOD_READ_CALCULATOR_T calculateRelativeSector, METHOD_READ_CALCULATOR_T calculateHead, METHOD_READ_CALCULATOR_T calculateTrack );
	int DefaultWriteSector( char* buffer, unsigned int sectorNumber, unsigned int amouontOfSectorsToRead, int deviceNumber );
	/*Read/Write Agnostic Stuff.*/
	typedef int ( *METHOD_READ_CALCULATOR_T )( int );
	int DefaultCalculateRelativeSector( int sector );
	int DefaultCalculateHead( int sector );
	int DefaultCalculateTrack( int sector );
	int GetCurrentSectorIODevice();
//	extern int ( *readSector )( char*, unsigned int, unsigned int, int );
//#endif
