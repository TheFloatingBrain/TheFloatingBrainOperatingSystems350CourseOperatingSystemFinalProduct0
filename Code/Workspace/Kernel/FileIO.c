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
#include "FileIO.h"

#define HAS_READ_DIRECTORY_M 1
#define HAS_NOT_READ_DIRECTORY_M 0
#define DIRECTORY_SECTOR_M 2
//#define FILE_NAME_SIZE_LIMIT_M 6
#define MAXIMUM_FILE_ENTRIES_M 16
//#define ENTRY_LENGTH_M 32
#define NO_SECTOR_M 0
#define MAP_SECTOR_M 1

#define READ_DIRECTORY_HELPER_M ReadDirectory( directory ) /*interrupt( 0x21, INTERRUPT_21_READ_SECTOR_M, directory, DIRECTORY_SECTOR_M, 1 )*/
#define READ_MAP_HELPER_M ReadMap( map ) /*interrupt( 0x21, INTERRUPT_21_READ_SECTOR_M, map, MAP_SECTOR_M, 1 )*/

unsigned short hasReadDirectory = HAS_NOT_READ_DIRECTORY_M;
char directory[ 512 ];
char map[ 512 ];

#define FILE_ENTRY_FORMULA_M( ENTRY, ITERATOR ) ( ( ENTRY * ENTRY_LENGTH_M ) + ITERATOR )


void ReadDirectory( DIRECTORY_POINTER_T writeDirectoryTo ) {
    interrupt( 0x21, INTERRUPT_21_READ_SECTOR_M, writeDirectoryTo, DIRECTORY_SECTOR_M, 1 );
}
void ReadMap( MAP_POINTER_T writeMapTo ) {
    interrupt( 0x21, INTERRUPT_21_READ_SECTOR_M, writeMapTo, MAP_SECTOR_M, 1 );
}

unsigned int ReadFile( char* fileName, char* buffer )
{
    unsigned int numberOfSectorsRead = 0;
    unsigned int i = 0;
    unsigned int entry = 0;
    unsigned int isFile = 1;
    char sector = 0;
    char* readBuffer = buffer;
    READ_DIRECTORY_HELPER_M;
    for( ; entry < MAXIMUM_FILE_ENTRIES_M; ++entry )
    {
        isFile = 1;
        for( i = 0; i < FILE_NAME_SIZE_LIMIT_M; ++i )
        {
            if( fileName[ i ] == '\0' )
            {
                if( directory[ FILE_ENTRY_FORMULA_M( entry, i ) ] != '\0' )
                    isFile = 0;
                break;
            }
           if( fileName[ i ] != directory[ FILE_ENTRY_FORMULA_M( entry, i ) ] ) {
                isFile = 0;
                break;
            }
        }
        if( isFile == 1 )
            break;
        else if( ( entry + 1 ) >= MAXIMUM_FILE_ENTRIES_M )
            return READ_FILE_DID_NOT_FIND_FILE_M;
    }
    for( i = FILE_NAME_SIZE_LIMIT_M; i < ENTRY_LENGTH_M; ++i )
    {
        sector = directory[ FILE_ENTRY_FORMULA_M( entry, i ) ];
        if( ( sector == NO_SECTOR_M ) && !( sector == MAP_SECTOR_M && i == FILE_NAME_SIZE_LIMIT_M ) )
            break;
        interrupt( 0x21, INTERRUPT_21_READ_SECTOR_M, readBuffer, sector, 1 );
        //DefaultReadSector( readBuffer, sector, 1, currentSectorIODevice );
        readBuffer += SECTOR_SIZE_M;
        ++numberOfSectorsRead;
    }
    return numberOfSectorsRead;
}

unsigned int ObtainSectors( char* fileName, char* thisDirectory, SECTOR_STORAGE_POINTER_T sectors )
{
    unsigned int j = 0;
    unsigned int foundFile = 1;
    unsigned int fileSizeInSectors = 0;
    unsigned int fileEntry;
    fileEntry = GetFileEntry( fileName, thisDirectory );
    if( fileEntry != FILE_ENTRY_NOT_FOUND_M )
    {
        for( j = FILE_NAME_SIZE_LIMIT_M; j < ENTRY_LENGTH_M; ++j )
        {
            if( thisDirectory[ FILE_ENTRY_FORMULA_M( fileEntry, j ) ] != NULL_TERMINATOR_M ) {
                sectors[ fileSizeInSectors ] = thisDirectory[ FILE_ENTRY_FORMULA_M( fileEntry, j ) ];
                ++fileSizeInSectors;
            }
        }
    }
    return fileSizeInSectors;
}

void DeleteFile( char* fileName )
{
    unsigned int numberOfSectors = 0;
    SECTOR_STORAGE_M( sectors );
    unsigned int i = 0;
    int fileEntry;
    READ_DIRECTORY_HELPER_M;
    fileEntry = GetFileEntry( fileName, directory );
    if( fileEntry != FILE_ENTRY_NOT_FOUND_M )
    {
        READ_MAP_HELPER_M;
        numberOfSectors = ObtainSectors( fileName, directory, sectors );
        for( ; i < numberOfSectors; ++i )
            map[ directory[ FILE_ENTRY_FORMULA_M( fileEntry, i ) ] ] = SECTOR_IS_FREE_M;
        directory[ FILE_ENTRY_FORMULA_M( fileEntry, 0 ) ] = NULL_TERMINATOR_M;
    }
    interrupt( 0x21, INTERRUPT_21_WRITE_SECTOR_M, directory, DIRECTORY_SECTOR_M, 1 );
    interrupt( 0x21, INTERRUPT_21_WRITE_SECTOR_M, map, MAP_SECTOR_M, 1 );
}

int GetFileEntry( char* fileName, char* thisDirectory )
{
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned char foundFile = 1;
    for( ; i < MAXIMUM_FILE_ENTRIES_M; ++i )
    {
        foundFile = 1;
        for( j = 0; j < FILE_NAME_SIZE_LIMIT_M; ++j ) {
            if( fileName[ j ] != thisDirectory[ FILE_ENTRY_FORMULA_M( i, j ) ] )
                foundFile = 0;
        }
        if( foundFile == 1 )
            break;
    }
    if( i == MAXIMUM_FILE_ENTRIES_M ) {
        /*PrintLine( "Error::KernelLibrary::int GetFileEntry( char*, char* ): Entry not found!" );*/
        return FILE_ENTRY_NOT_FOUND_M;
    }
    return i;
}

unsigned int WriteFile( char* fileName, char* buffer, unsigned int numberOfSectors )
{
    char swapSector[ SECTOR_SIZE_M ];
    unsigned int entry = 0;
    unsigned int i = 0;
    unsigned int cursor = 0;
    unsigned int swapIterator = 0;
    unsigned int sectorsFound = 0;
    unsigned char range = 0;

    char line[ 5 ];
    char begin[ 2 ];
    char end[ 2 ];

    begin[ 0 ] = '[';
    begin[ 1 ] = '\0';

    end[ 0 ] = ']';
    end[ 1 ] = '\0';

    /*Ranged write mode*/
    if( fileName[ 0 ] == '\0' ) {
        range = *( ++fileName );
        ++fileName;
    }
    READ_DIRECTORY_HELPER_M;
    READ_MAP_HELPER_M;
    i = 0;
    if( ( entry = GetFileEntry( fileName, directory ) ) == FILE_ENTRY_NOT_FOUND_M )
        for( entry = 0; entry < MAXIMUM_FILE_ENTRIES_M && directory[ FILE_ENTRY_FORMULA_M( entry, 0 ) ] != NULL_TERMINATOR_M; ++entry );
    if( entry >= MAXIMUM_FILE_ENTRIES_M ) {
        PrintLine( "Error::KernelLibrary::unsigned int WriteFile( char*, char*, unsigned int ): Out of space on device!" );
        return NO_SPACE_ON_DEVICE_M;
    }
    for( ; i < FILE_NAME_SIZE_LIMIT_M && fileName[ i ] != '\0'; directory[ FILE_ENTRY_FORMULA_M( entry, i++ ) ] = fileName[ i ] );
    for( i = 0; i < SECTOR_SIZE_M && sectorsFound <= numberOfSectors; ++i )
    {
        if( map[ i ] == SECTOR_IS_FREE_M )
        {
            map[ i ] = SECTOR_IN_USE_M;
            directory[ FILE_ENTRY_FORMULA_M( entry, ( sectorsFound++ ) + FILE_NAME_SIZE_LIMIT_M ) ] = i;
            /*Zero out the buffer.*/
            for( swapIterator = 0; swapIterator < SECTOR_SIZE_M; swapSector[ swapIterator++ ] = '\0' );
            for( swapIterator = 0; swapIterator < SECTOR_SIZE_M && 
                    ( ( range == 0 ) ? ( buffer[ cursor ] != NULL_TERMINATOR_M ) : ( swapIterator < range ) ); 
                    ++swapIterator )
                swapSector[ swapIterator ] = buffer[ cursor++ ];
            interrupt( 0x21, INTERRUPT_21_WRITE_SECTOR_M, swapSector, i, 1 );
        }
    }
    interrupt( 0x21, INTERRUPT_21_WRITE_SECTOR_M, directory, DIRECTORY_SECTOR_M, 1 );
    interrupt( 0x21, INTERRUPT_21_WRITE_SECTOR_M, map, MAP_SECTOR_M, 1 );
    return WROTE_FILE_M;
}
