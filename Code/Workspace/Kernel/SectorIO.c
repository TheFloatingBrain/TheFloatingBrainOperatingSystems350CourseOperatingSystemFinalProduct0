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
//Read Sector Stuff//
int ReadSectorChooseMethodToRead( char* buffer, unsigned int sectorNumber, unsigned int amouontOfSectorsToRead, int deviceNumber, 
		METHOD_READ_CALCULATOR_T calculateRelativeSector, METHOD_READ_CALCULATOR_T calculateHead, METHOD_READ_CALCULATOR_T calculateTrack )
{
	return interrupt( SECTOR_IO_INTERRUPT_M, WRITE_PARAMETER_M( amouontOfSectorsToRead, SECTOR_IO_READ_FUNCTION_M ), 
			buffer, WRITE_PARAMETER_M( calculateRelativeSector( sectorNumber ), calculateTrack( sectorNumber ) ), 
			WRITE_PARAMETER_M( deviceNumber, calculateHead( sectorNumber ) ) );
}
int DefaultReadSector( char* buffer, unsigned int sectorNumber, unsigned int amouontOfSectorsToRead, int deviceNumber ) {
	return ReadSectorChooseMethodToRead( buffer, sectorNumber, amouontOfSectorsToRead, deviceNumber, 
			DefaultCalculateRelativeSector, DefaultCalculateHead, DefaultCalculateTrack );
}
//Write Sector Stuff//
int WriteSectorChooseMethodToWrite( char* buffer, unsigned int sectorNumber, unsigned int amouontOfSectorsToRead, int deviceNumber, 
		METHOD_READ_CALCULATOR_T calculateRelativeSector, METHOD_READ_CALCULATOR_T calculateHead, METHOD_READ_CALCULATOR_T calculateTrack )
{
	return interrupt( SECTOR_IO_INTERRUPT_M, WRITE_PARAMETER_M( amouontOfSectorsToRead, SECTOR_IO_WRITE_FUNCTION_M ), 
			buffer, WRITE_PARAMETER_M( calculateRelativeSector( sectorNumber ), calculateTrack( sectorNumber ) ), 
			WRITE_PARAMETER_M( deviceNumber, calculateHead( sectorNumber ) ) );
}
int DefaultWriteSector( char* buffer, unsigned int sectorNumber, unsigned int amouontOfSectorsToRead, int deviceNumber ) {
	return WriteSectorChooseMethodToWrite( buffer, sectorNumber, amouontOfSectorsToRead, deviceNumber, 
			DefaultCalculateRelativeSector, DefaultCalculateHead, DefaultCalculateTrack );
}

int DefaultCalculateRelativeSector( int sector ) {
	return ( Modulus( sector, 18 ) + 1 );
}
int DefaultCalculateHead( int sector ) {
	return Modulus( Divide( sector, 18 ), 2 );
}
int DefaultCalculateTrack( int sector ) {
	return Divide( sector, 36 );
}
int GetCurrentSectorIODevice()
{
	int sectorIODevice = 0;
	interrupt( 0x21, INTERRUPT_21_GET_SECTOR_IO_DEVICE_NUMBER, &sectorIODevice, 0, 0 );
	return sectorIODevice;
}

//int ( *readSector )( char*, unsigned int, unsigned int, int ) = DefaultReadSector;
