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

#define READ_FILE_FOUND_FILE_M 1
#define READ_FILE_DID_NOT_FIND_FILE_M 0
#define FILE_ENTRY_NOT_FOUND_M ( -1 )
#define SECTOR_IS_FREE_M 0x0
#define SECTOR_IN_USE_M 0xFF
/*TODO: Make this more relevent.*/
#define NO_SPACE_ON_DEVICE_M 1
#define WROTE_FILE_M 0

/*TODO: Make function to dynamically determine file size in sectors!*/
/*TODO: Make function to dynamically determine file name size.*/

/*Returnes READ_FILE_DID_NOT_FIND_FILE_M or any positive integer 
indicating the number of sectors read (may also return zero if 
file was found but no sector was read TODO: change this?)*/

unsigned int ReadFile( char* fileName, char* buffer );

unsigned int ObtainSectors( char* fileName, char* thisDirectory, SECTOR_STORAGE_POINTER_T sectors );

void ReadDirectory( DIRECTORY_POINTER_T writeDirectoryTo );
void ReadMap( MAP_POINTER_T writeMapTo );

void DeleteFile( char* fileName );

int GetFileEntry( char* fileName, char* thisDirectory );


/******************************************************************************
* : To use ranged writing mode in the file name make: *************************
*** * : First char indicates '\0' to inidicate that ranged writing mode is on *
*** * : The second char be how many char's should be copied into the last *****
*** sector from the buffer ****************************************************
******************************************************************************/
unsigned int WriteFile( char* fileName, char* buffer, unsigned int numberOfSectors );
