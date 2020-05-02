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

#define COMMAND_LINE_TYPE_COMMAND_M "type"
#define COMMAND_LINE_EXECUTE_COMMAND_M "execute"
#define COMMAND_LINE_DELETE_COMMAND_M "delete"
#define COMMAND_LINE_COPY_COMMAND_M "copy"
#define COMMAND_LINE_CREATE_COMMAND_M "create"
#define AMOUNT_OF_COMMANDS_M 8//4
#define COMMAND_LINE_AMOUNT_OF_COMMANDS_M 8
#define COMMAND_LINE_WORD_LENGTH_M 64
#define COMMAND_LINE_MAX_LENGTH_M ( COMMAND_LINE_WORD_LENGTH_M * COMMAND_LINE_AMOUNT_OF_COMMANDS_M )
#define STARTING_EXECUTE_LOAD_ADDRESS_M 0x2000

#define MAX_FILE_SIZE_M SECTOR_SIZE_M * ( ENTRY_LENGTH_M - FILE_NAME_SIZE_LIMIT_M )

#define BIOS_READ_KEYBOARD_LINE_M 0x16
#define BIOS_CURSOR_INTERRUPT_M 0x10

#define DYNAMIC_MEMORY_SIZE_M 512 * 26
unsigned int shellDynamicMemory[ DYNAMIC_MEMORY_SIZE_M ];

void SetMemory( char* memory, char to, unsigned int length );

typedef void( *COMMAND_FUNCTION_T )( char* );

struct Command {
    COMMAND_FUNCTION_T command;
    char* keyword;
};

void TypeCommand( char* file );
void ExecuteCommand( char* program );
void DeleteFile( char* fileName );
void CopyCommand( char* fileName );
void CreateCommand( char* fileName );

void TESTSETCOMMAND( char* what );
void TESTGETCOMMAND( char* destination );

void DIR( char* arg );

COMMAND_FUNCTION_T commandFunctions[ AMOUNT_OF_COMMANDS_M ] = 
{ 
    TypeCommand, 
    ExecuteCommand, 
    DeleteFile, 
    CopyCommand, 
    CreateCommand,
    TESTSETCOMMAND, 
    TESTGETCOMMAND,
    DIR
};

char* commandNames[ AMOUNT_OF_COMMANDS_M ] = 
{
    COMMAND_LINE_TYPE_COMMAND_M, 
    COMMAND_LINE_EXECUTE_COMMAND_M, 
    COMMAND_LINE_DELETE_COMMAND_M, 
    COMMAND_LINE_COPY_COMMAND_M, 
    COMMAND_LINE_CREATE_COMMAND_M,
   "set", 
   "get",
   "dir"
};

struct Command commands[ AMOUNT_OF_COMMANDS_M ];

void InitializeCommands();

void TestShiftKey();

void Experiment( char* upOrDown );

void Clear();

main()
{
    char userInput[ COMMAND_LINE_MAX_LENGTH_M ];
    char arguments[ COMMAND_LINE_MAX_LENGTH_M ];
    unsigned int width = COMMAND_LINE_WORD_LENGTH_M;
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int stringLength = 0;
    unsigned int match = 0;
    struct Command* currentCommand = 0;
    DynamicMemoryStart( shellDynamicMemory, DYNAMIC_MEMORY_SIZE_M );
    InitializeCommands();
    //Clear();
//    TextEditor( 79 );
    while( 1 )
    {
        interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "Shell: ", INTERRUPT_21_PRINT_STRING_PRINT_STRING_M, 0 );
        interrupt( 0x21, INTERRUPT_21_READ_STRING_FROM_KEYBOARD_M, userInput, 
                INTERRUPT_21_READ_STRING_FROM_KEYBOARD_READ_RANGE_M, COMMAND_LINE_MAX_LENGTH_M, 0 );
        BufferedSplitByWhiteSpaceRanged( arguments, userInput, width, 0, FindStringLength( userInput ) );
        for( i = 0; i < AMOUNT_OF_COMMANDS_M; ++i )
        {
            stringLength = FindStringLength( commands[ i ].keyword );
            for( j = 0; j < ( stringLength - 1 ); ++j )
            {
                if( commands[ i ].keyword[ j ] == '\0' || arguments[ j ] == '\0' || 
                        commands[ i ].keyword[ j ] != arguments[ j ] ) {
                    match = 0;
                    break;
                }
                if( j + 1 == ( stringLength - 1 ) )
                    match = 1;
            }
            if( match == 1 ) {
                currentCommand = &commands[ i ];
                break;
            }
        }
        if( match == 1 )
            currentCommand->command( &arguments[ width ] );
        match = 0;
        MemorySet( arguments, '\0', COMMAND_LINE_MAX_LENGTH_M );
    }
    while( 1 );
}

void SetMemory( char* memory, char to, unsigned int length )
{
    while( length > 0 )
    {
        *memory = to;
        ++memory;
        --length;
    }
}

void InitializeCommands()
{
    unsigned int i = 0;
    for( ; i < AMOUNT_OF_COMMANDS_M; ++i ) {
        commands[ i ].command = commandFunctions[ i ];
        commands[ i ].keyword = commandNames[ i ];
    }
}

void TypeCommand( char* file )
{
    /*TODO: Use sector size to do this.*/
    int fileLength = 0;
    char maxFile[ MAX_FILE_SIZE_M ];
    SetMemory( maxFile, NULL_TERMINATOR_M, MAX_FILE_SIZE_M );
    interrupt( 0x21, INTERRUPT_21_READ_FILE_M, file, maxFile, &fileLength );
    //interrupt( 0x21, INTERRUPT_21_PRINT_STRING_PRINT_STRING_M, maxFile, INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
    interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, maxFile, INTERRUPT_21_PRINT_STRING_PRINT_RANGE_LINE_M, fileLength * SECTOR_SIZE_M );
}
void ExecuteCommand( char* program )
{
    unsigned int processesRunning = 1;
    interrupt( 0x21, INTERRUPT_21_PROCESSES_RUNNING_M, &processesRunning, 0, 0 );
    /*TODO: Use ReadFile to determine number of sectors read!!!*/
    interrupt( 0x21, INTERRUPT_21_EXECUTE_PROGRAM, program, STARTING_EXECUTE_LOAD_ADDRESS_M + ( ( processesRunning + 1 ) * MAX_FILE_SIZE_M ), 0 );
}
void DeleteFile( char* fileName ) {
    interrupt( 0x21, INTERRUPT_21_DELETE_FILE_M, ( int ) fileName, 0, 0 );
}
void CopyCommand( char* fileName )
{
    int fileLength = 0;
    unsigned int iterator = 0;
    char* fileBuffer = 0;
    char name[ 9 ];
    unsigned int lengthInChars;
    SECTOR_STORAGE_M( sectors );
    name[ 0 ] = '\0';
    name[ 1 ] = 255;
    name[ 2 ] = ( fileName + COMMAND_LINE_WORD_LENGTH_M )[ 0 ];
    name[ 3 ] = ( fileName + COMMAND_LINE_WORD_LENGTH_M )[ 1 ];
    name[ 4 ] = ( fileName + COMMAND_LINE_WORD_LENGTH_M )[ 2 ];
    name[ 5 ] = ( fileName + COMMAND_LINE_WORD_LENGTH_M )[ 3 ];
    name[ 6 ] = ( fileName + COMMAND_LINE_WORD_LENGTH_M )[ 4 ];
    name[ 7 ] = ( fileName + COMMAND_LINE_WORD_LENGTH_M )[ 5 ];
    name[ 8 ] = '\0';
    interrupt( 0x21, INTERRUPT_21_OBTAIN_FILE_SECTORS_M, fileName, sectors, &fileLength );
    lengthInChars = ( sizeof( char ) * fileLength * SECTOR_SIZE_M );
    fileBuffer = DynamicAllocate( lengthInChars );
    MemorySet( fileBuffer, '\0', lengthInChars );
    interrupt( 0x21, INTERRUPT_21_READ_FILE_M, fileName, fileBuffer, &fileLength );
    interrupt( 0x21, INTERRUPT_21_WRITE_FILE_M, name, fileBuffer, fileLength );
    Deallocate( fileBuffer );
}


#define TEST_KEY_OUT_SIZE_TEST_SHIFT_KEY_M 10
void TestShiftKey()
{
    unsigned int testKey = 0;
    char testKeyOut[ TEST_KEY_OUT_SIZE_TEST_SHIFT_KEY_M ];
    while( 1 )
    {
        MemorySet( testKeyOut, '$', TEST_KEY_OUT_SIZE_TEST_SHIFT_KEY_M );
        interrupt( 0x21, INTERRUPT_21_CONTROL_KEYS_STATUS_M, &testKey, 0, 0 );
        NumberToString( ( &testKeyOut[ 0 ] ) + 1, testKey );
        testKeyOut[ 0 ] = '\r';
        testKeyOut[ TEST_KEY_OUT_SIZE_TEST_SHIFT_KEY_M - 1 ] = '\0';
        interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, testKeyOut, INTERRUPT_21_PRINT_STRING_PRINT_STRING_M, 0 );
    }
}

void CreateCommand( char* fileName )
{
    char toSet[ 16 ];
    toSet[ 0 ] = 't';
    toSet[ 1 ] = 'x';
    toSet[ 2 ] = 't';
    toSet[ 3 ] = 'e';
    toSet[ 4 ] = 'd';
    toSet[ 5 ] = 't';
    toSet[ 6 ] = 'c';
    toSet[ 7 ] = 'm';
    toSet[ 8 ] = 'd';
    toSet[ 9 ] = fileName[ 0 ];
    toSet[ 10 ] = fileName[ 1 ];
    toSet[ 11 ] = fileName[ 2 ];
    toSet[ 12 ] = fileName[ 3 ];
    toSet[ 13 ] = fileName[ 4 ];
    toSet[ 14 ] = fileName[ 5 ];
    toSet[ 15 ] = '\0';
    TESTSETCOMMAND( toSet );
    ExecuteCommand( "txtedt" );
}

void Experiment( char* upOrDown )
{
}

void Clear() {
    interrupt( 0x10, WRITE_PARAMETER_M( 0x03, 0x00 ), 0, 0, 0 );
}

void TESTSETCOMMAND( char* what ) {
    interrupt( 0x21, INTERRUPT_21_COMMAND_LINE_ARGUMENT_SET_M, what, FindStringLength( what ), 0 );
}
void TESTGETCOMMAND( char* destination ) {
    interrupt( 0x21, INTERRUPT_21_COMMAND_LINE_ARGUMENT_GET_M, destination, 0, 0 );
    interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, destination, INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
}

#define FILE_ENTRY_FORMULA_SHELL_M( ENTRY, ITERATOR ) ( ( ENTRY * ENTRY_LENGTH_M ) + ITERATOR )
void DIR( char* arg )
{
    DIRECTORY_STORAGE_TYPE_M( readDirectory );
    char entries = 0, i = 0;
    char name[ 7 ];
    name[ 6 ] = '\0';
    interrupt( 0x21, INTERRUPT_21_READ_DIRECTORY_M, readDirectory, 0, 0 );
    for( ; entries < 16; ++entries )
    {
        for( i = 0; i < 6; ++i )
            name[ i ] = readDirectory[ FILE_ENTRY_FORMULA_SHELL_M( entries, i ) ];
        if( name[ 0 ] != NULL_TERMINATOR_M )
            interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, name, INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
    }
}
