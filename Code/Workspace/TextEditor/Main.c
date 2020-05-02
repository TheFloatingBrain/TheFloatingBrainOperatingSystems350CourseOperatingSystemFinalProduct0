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
#include "../KernelLibrary/DynamicMemory.h"
#include "../KernelLibrary/Math/Math.h"

#define DYNAMIC_MEMORY_SIZE_M 512 * 55

unsigned int textEditorDynamicMemory[ DYNAMIC_MEMORY_SIZE_M ];    


struct EditorTab
{
    char* text;
    char cursorX, cursorY, cursorShown, commandLineCreateCommandMode;
    unsigned int line, lines, 
            lineLength, bufferLength, fileLength, simultanousLines;
    char* fileName;
    //Would mark this private if I could.//
    char fileNameBuffer[ FILE_NAME_SIZE_LIMIT_M + 1 ];
};

void InitializeEditorTab( struct EditorTab* toInitialize, unsigned int lineLength, unsigned int simultanousLines, char* fileName, char commandLineCreateCommandMode );

void Clear();

void MoveUpLine( struct EditorTab* tab );

void MoveDownLine( struct EditorTab* tab );

void TextEditorResetCursorX( struct EditorTab* tab );

void AddLine( struct EditorTab* tab );

void Insert( struct EditorTab* tab, char x, char line, char what, char setCursor );

void Backspace( struct EditorTab* tab );

#define SCROLL_LINE_UP_M 1
#define SCROLL_LINE_DOWN_M 0

void ScrollLine( struct EditorTab* tab, unsigned int fromLine, char direction );

void CommandCursor( struct EditorTab* tab, char currentInput );

void Type( struct EditorTab* tab, char currentInput, char makeNewLine );

void TextEditor( struct EditorTab* tab );

void SaveFile( struct EditorTab* tab );

void DetermineModeAndFileNameFromCommandLine( char* fileName, char* commandLineCreateCommandMode );

main()
{
    struct EditorTab defaultTab;
    unsigned int test =0;
    char commandLineFileName[ FILE_NAME_SIZE_LIMIT_M + 1 ];
    commandLineFileName[ 0 ] = '\0';
    commandLineFileName[ 1 ] = 'd';
    commandLineFileName[ 2 ] = 'e';
    commandLineFileName[ 3 ] = 'f';
    commandLineFileName[ 4 ] = 'l';
    commandLineFileName[ 5 ] = 't';
    commandLineFileName[ 6 ] = '\0';
    commandLineFileName[ FILE_NAME_SIZE_LIMIT_M ] = NULL_TERMINATOR_M;
    DynamicMemoryStart( textEditorDynamicMemory, DYNAMIC_MEMORY_SIZE_M );
    DetermineModeAndFileNameFromCommandLine( commandLineFileName, &defaultTab.commandLineCreateCommandMode );
    InitializeEditorTab( &defaultTab, 80, 25, commandLineFileName, defaultTab.commandLineCreateCommandMode ); //10, 4 );// 80, 25 );
    TextEditor( &defaultTab );
    interrupt( 0x21, INTERRUPT_21_TERMINATE_M, 0, 0, 0 );
}

/*ReadLineFromKeyboard Stuff.*/
#define BIOS_READ_KEYBOARD_LINE_M 0x16
#define ENTER_KEY_M 0xD
#define LINE_FEED_M 0xA
#define BACKSPACE_KEY_M 0x8
#define READ_LINE_FROM_KEYBOARD_UNLIMITED_LENGTH_M -1
#define READ_LINE_FROM_KEYBOARD_MINIMUM_LENGTH_M 80

/*****************************/

#define CONTROL_KEY_M 4
#define MOVE_CURSOR_LEFT_M 1
#define MOVE_CURSOR_DOWN_M 19
#define MOVE_CUROR_RIGHT_M 4
#define MOVE_CURSOR_UP_M 23
#define SHOW_CURSOR_M 3
#define SAVE_FILE_KEY_M 15
#define EXIT_KEY_M 24

#define ACCESS_TEXT_AT_M( columb, row ) tab->text[ ( ( ( row ) * tab->bufferLength ) + ( columb ) ) ]

#define CURSOR_CHARECHTER_M '$'

#define ABS( value ) ( value < 0 ? -value : value )

void InitializeEditorTab( struct EditorTab* toInitialize, unsigned int lineLength, unsigned int simultanousLines, char* fileName, char commandLineCreateCommandMode )
{
    toInitialize->lineLength = lineLength;
    toInitialize->simultanousLines = simultanousLines;
    toInitialize->bufferLength = ( lineLength ) + 1;
    toInitialize->fileLength = toInitialize->bufferLength;
    toInitialize->text = DynamicAllocate( toInitialize->fileLength );
    toInitialize->cursorX = 0;
    toInitialize->cursorY = 0;
    toInitialize->lines = 1;
    toInitialize->cursorShown = 0;
    toInitialize->fileName = toInitialize->fileNameBuffer;
    toInitialize->fileName[ 0 ] = NULL_TERMINATOR_M;
    /*Reduce reuse recycle.*/
    for( toInitialize->line = 0; toInitialize->line < FILE_NAME_SIZE_LIMIT_M; ++toInitialize->line )
        toInitialize->fileName[ toInitialize->line ] = fileName[ toInitialize->line ];
    fileName[ FILE_NAME_SIZE_LIMIT_M ] = NULL_TERMINATOR_M;
    /*Actual default value this needs to be.*/
    toInitialize->line = 1;
    toInitialize->commandLineCreateCommandMode = commandLineCreateCommandMode;
}


void Clear() {
    interrupt( 0x10, WRITE_PARAMETER_M( 0x03, 0x00 ), 0, 0, 0 );
}

void TextEditorResetCursorX( struct EditorTab* tab )
{
    for( tab->cursorX = ( tab->lineLength - 1 ); tab->cursorX >= 0; --tab->cursorX ) {
        if( ACCESS_TEXT_AT_M( tab->cursorX, tab->line - 1 ) != '\0' )
            break;
    }
}

void MoveUpLine( struct EditorTab* tab )
{
    if( tab->cursorY == 0 )
        ScrollLine( tab, tab->line, SCROLL_LINE_UP_M );
    else
        --tab->cursorY;
    --tab->line;
}

void MoveDownLine( struct EditorTab* tab )
{
    if( tab->cursorY == ( tab->simultanousLines - 1 ) )
        ScrollLine( tab, tab->line, SCROLL_LINE_DOWN_M );
    else
        ++tab->cursorY;
    ++tab->line;
}

void AddLine( struct EditorTab* tab )
{
    unsigned int i = 0;
    char* swapBuffer = tab->text;
    tab->text = DynamicAllocate( tab->bufferLength + tab->fileLength );
    MemorySet( tab->text, '\0', tab->fileLength + tab->bufferLength );
    for( i = 0; i < tab->fileLength; ++i )
        tab->text[ i ] = swapBuffer[ i ];
    tab->fileLength += tab->bufferLength;
    Deallocate( swapBuffer );
    swapBuffer = 0;
    ++tab->lines;
}


void Insert( struct EditorTab* tab, char x, char line, char what, char setCursor )
{
    char previousBuffer = what;
    char currentBuffer = ACCESS_TEXT_AT_M( x, line );
    char* displayLine = DynamicAllocate( tab->lineLength + 2 );
    char beginLine = line;
    displayLine[ tab->lineLength + 1 ] = '\0';
    while( line <= tab->lines )
    {
        if( ( x + 1 ) == ( tab->lineLength ) )
        {
            ++line;
            x = 0;
            if( line > tab->lines ) {
                AddLine( tab );
                break;
            }
        }
        else if( ACCESS_TEXT_AT_M( x, line - 1 ) == '\0' )
        {
            if( ( line + 1 ) > tab->lines )
                break;
            ++line;
            x = 0;
        }
        else
            ++x;
        currentBuffer = ACCESS_TEXT_AT_M( x, line - 1 );
        ACCESS_TEXT_AT_M( x, line - 1 ) = previousBuffer;
        previousBuffer = currentBuffer;
//        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x, line - 1, ACCESS_TEXT_AT_M( x, line - 1 ) );
    }
    /*Reuse the chars as iterators and print all the new stuff out.*/
    for( previousBuffer = ( beginLine - 1 ); previousBuffer < line; ++previousBuffer )
    {
        for( currentBuffer = 0; currentBuffer <= tab->lineLength; ++currentBuffer )
            displayLine[ currentBuffer ] = ACCESS_TEXT_AT_M( currentBuffer, previousBuffer );
        interrupt( 0x21, INTERRUPT_21_SET_CURSOR_LOCATION_M, 0, previousBuffer, 0 );
        interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, displayLine, INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
    }
    Deallocate( displayLine );
}

void Backspace( struct EditorTab* tab )
{
    char x = 0;
    unsigned int j = 0, i = 0;
    if( ( ( int ) tab->cursorX ) - 1 < 0 )
    {
        if( tab->line > 1 )
        {
            MoveUpLine( tab );
            if( ACCESS_TEXT_AT_M( tab->cursorX, tab->line - 1 ) == '\0' )
                TextEditorResetCursorX( tab );
            else
                tab->cursorX = ( tab->lineLength - 1 );
            return;
        }
        x = tab->cursorX;
    }
    else
        x = tab->cursorX--;
    for( j = tab->cursorY; j < tab->lines; ++j )
    {
        for( i = x; i < tab->lineLength; ++i )
        {
            if( ( i + 1 ) >= tab->lineLength )
            {
                if( ( j + 1 ) < tab->lines ) {
                    tab->text[ ( ( j * tab->bufferLength ) + i ) ] = 
                            tab->text[ ( ( j + 1 ) * tab->bufferLength ) ];
                }
                else {
                    tab->text[ ( ( j * tab->bufferLength ) + i ) ] = '\0';
                }
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, i, j, 
                        tab->text[ ( ( j * tab->bufferLength ) + i ) ] );
                break;
            }
            else
            {
                tab->text[ ( ( j * tab->bufferLength ) + i ) ] = 
                        tab->text[ ( ( j * tab->bufferLength ) + ( i + 1 ) ) ];
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, i, j, 
                        tab->text[ ( ( j * tab->bufferLength ) + i ) ] );
            }
            
        }
        x = 0;
    }
}

#define SCROLL_LINE_INNER_LOOP_M \
    for( x = 0; x <= tab->lineLength; ++x ) \
        displayLine[ x ] = ACCESS_TEXT_AT_M( x, y - 1 ); \
    interrupt( 0x21, INTERRUPT_21_SET_CURSOR_LOCATION_M, 0, cursorLine++, 0 ); \
    interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, displayLine, \
            INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );

void ScrollLine( struct EditorTab* tab, unsigned int fromLine, char direction )
{
    /*Assumes the lines ( line - 1 ) to ( line - 1 ) - tab->simultanousLines are being displayed.*/
    unsigned int x = 0, y = 0;
    unsigned int maximumLine = 0;
    unsigned int cursorLine = tab->simultanousLines - 1;
    //char* displayLine = DynamicAllocate( tab->lineLength + 2 );
    //displayLine[ tab->lineLength + 1 ] = '\0';
    if( direction == SCROLL_LINE_DOWN_M && tab->lines > fromLine )
    {
        for( y = fromLine; y > ( fromLine - tab->simultanousLines ); --y ) {
            for( x = 0; x < tab->lineLength; ++x )
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x, cursorLine, ACCESS_TEXT_AT_M( x, y - 1 ) );
            --cursorLine;
      //      SCROLL_LINE_INNER_LOOP_M
        }
    }
    else if( direction == SCROLL_LINE_UP_M && fromLine > 1 )
    {
        maximumLine = ( ( fromLine + tab->simultanousLines ) - 1 ) <= tab->lines ? 
                ( fromLine + tab->simultanousLines ) : tab->lines;
        for( y = fromLine; y < maximumLine; ++y ) {
            for( x = 0; x < tab->lineLength; ++x )
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x, cursorLine, ACCESS_TEXT_AT_M( x, y - 1 ) );
            --cursorLine;
        //    SCROLL_LINE_INNER_LOOP_M
        }
    }
    //Deallocate( displayLine );
}

/*Thought I would save this debugging message ;) 'case I want to use it again :P*/
/*                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x, line + 10, 'G' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+1, line + 10, 'I' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+2, line + 10, 'G' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+3, line + 10, 'A' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+4, line + 10, ' ' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+5, line + 10, 'D' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+6, line + 10, 'R' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+7, line + 10, 'I' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+8, line + 10, 'L' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+9, line + 10, 'L' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+10, line + 10, ' ' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+11, line + 10, 'B' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+12, line + 10, 'R' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+13, line + 10, 'E' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+14, line + 10, 'A' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+15, line + 10, 'K' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+16, line + 10, '!' );
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, x+17, line + 10, '!' );*/

void CommandCursor( struct EditorTab* tab, char currentInput )
{
    char iterator = 0;
    char userInput = 0;
    unsigned int previousCursorX, previousCursorY;
    //interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 0, 12, currentInput + 48 );
    switch( currentInput )
    {
        case MOVE_CURSOR_LEFT_M:
        {
            if( ( tab->cursorX - 1 ) >= 0 )
                --tab->cursorX;
            else if( ( ( ( int ) tab->cursorY ) - 1 ) >= 0 ) {
                MoveUpLine( tab );
                tab->cursorX = ( tab->lineLength - 1 );
            }
            break;
        }
        case MOVE_CUROR_RIGHT_M:
        {
            if( ( tab->cursorX + 1 ) < tab->lineLength ) {
                if( ACCESS_TEXT_AT_M( tab->cursorX, tab->line - 1 ) != '\0' && ACCESS_TEXT_AT_M( tab->cursorX + 1, tab->line - 1 ) == '\0' || 
                            ACCESS_TEXT_AT_M( tab->cursorX, tab->line - 1 ) != '\0' && ACCESS_TEXT_AT_M( tab->cursorX + 1, tab->line - 1 ) != '\0' )
                        ++tab->cursorX;
            }
            else if( tab->line <= tab->lines ) {
                MoveDownLine( tab );
                tab->cursorX = 0;
            }
            break;
        }
        case MOVE_CURSOR_UP_M:
        {
            if( ( ( ( int ) tab->cursorY ) - 1 ) >= 0 )
            {
                MoveUpLine( tab );
                if( ACCESS_TEXT_AT_M( tab->cursorX, tab->line - 1 ) == '\0' )
                    TextEditorResetCursorX( tab );
            }
            break;
        }
        case MOVE_CURSOR_DOWN_M:
        {
            if( ( tab->cursorY + 1 ) < tab->lines )
            {
                MoveDownLine( tab );
                if( ACCESS_TEXT_AT_M( tab->cursorX, tab->line - 1 ) == '\0' )
                    TextEditorResetCursorX( tab );
            }
            break;
        }
        case SHOW_CURSOR_M: {
            interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, tab->cursorX, tab->cursorY, CURSOR_CHARECHTER_M );
            tab->cursorShown = 1;
            break;
        }
        case SAVE_FILE_KEY_M:
        {
            /*Get the job done*/
            if( tab->fileName[ 0 ] == NULL_TERMINATOR_M )
            {
                previousCursorX = tab->cursorX;
                previousCursorY = tab->cursorY;
                interrupt( 0x21, INTERRUPT_21_SET_CURSOR_LOCATION_M, 0, tab->simultanousLines - 1, 0 );
                interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, 
                        "Please enter name for file: ", INTERRUPT_21_PRINT_STRING_PRINT_STRING_M, 0 );
                for( iterator = 0; iterator < FILE_NAME_SIZE_LIMIT_M; ++iterator )
                {
                    interrupt( 0x21, INTERRUPT_21_READ_CHARECHTER_FROM_KEYBOARD_M, &userInput, 0, 0 );
                    if( userInput >= 32 && userInput <= 126 ) {
                        tab->fileName[ iterator ] = userInput;
                        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, iterator + 29, tab->simultanousLines - 1, userInput );
                    }
                }
            }
            SaveFile( tab );
            break;
        }
        case EXIT_KEY_M:
        {
            Deallocate( tab->text );
            interrupt( 0x21, INTERRUPT_21_TERMINATE_M, 0, 0, 0 );
            break;
        }
        default:
            break;
    }
}

void Type( struct EditorTab* tab, char currentInput, char makeNewLine )
{
    unsigned int i = 0;
    if( tab->cursorX <= ( tab->lineLength - 1 ) )
    {
        if( currentInput == BACKSPACE_KEY_M )
            Backspace( tab );
        else
        {
            if( ACCESS_TEXT_AT_M( tab->cursorX, tab->line - 1 ) == '\0' )
            {
                ACCESS_TEXT_AT_M( tab->cursorX, tab->line - 1 ) = currentInput;
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, tab->cursorX, tab->cursorY, ACCESS_TEXT_AT_M( tab->cursorX, tab->line - 1 ) );
                ++tab->cursorX;
            }
            else
                Insert( tab, tab->cursorX, tab->line, currentInput, 0 );
        }
    }
    /*else */if( tab->cursorX > ( tab->lineLength - 1 ) )
    {
        /*For debugging
        for( i = 0; i < tab->lineLength; ++i ) {
            interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, i, 10, 
                    tab->text[ ( tab->fileLength - tab->bufferLength ) + i ] );
        }
        */
        if( ( tab->line + 1 ) > tab->lines || makeNewLine == 1 )
            AddLine( tab );
        tab->cursorX = 0;
        MoveDownLine( tab );
    }
}

/**********************************************************************
* TODO?: Work in progress, need to work on physics homework :( ********
* I might work on this on my free time or if I have time. :( I ********
* already spent too much time trying to get the interrupts to work :( *
* Since this is something I may have to come back to, I have **********
* commented it out because it made the shell too big, and was *********
* causing bugs. *******************************************************
**********************************************************************/



/*
* : TODO: 
    * Insert
    * Paging
    * --------------
    * Copy/Paste?
    * Copy/Paste buffer in OS?
*/

void TextEditor( struct EditorTab* tab )
{
    int makeNewLine = 0;
    unsigned int x = 0, y = 0, i = 0, j = 0, currentControlKeyState = 0;
    char currentInput;
    char* swapBuffer = 0;
    Clear();
    while( 1 )
    {
        if( tab->cursorShown != 0 ) {
            interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, tab->cursorX, tab->cursorY, ACCESS_TEXT_AT_M( tab->cursorX, tab->cursorY ) );
            tab->cursorShown = 0;
        }
        interrupt( 0x21, INTERRUPT_21_READ_CHARECHTER_FROM_KEYBOARD_M, &currentInput, 0, 0 );
        interrupt( 0x21, INTERRUPT_21_CONTROL_KEYS_STATUS_M, &currentControlKeyState, 0, 0 );
        switch( currentControlKeyState )
        {
            case CONTROL_KEY_M: {
                CommandCursor( tab, currentInput );
                break;
            }
            default:
            {
                if( currentInput >= 32 || currentInput == BACKSPACE_KEY_M || currentInput == ENTER_KEY_M )
                    Type( tab, currentInput, makeNewLine );
                break;
            }
        }
        /*
        for( i = 0; i < lines; ++i )
        {
            for( j = 0; j < lineLength; ++j )
                interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, j, i, ACCESS_tab->text_AT_M( j, i ) );
        }*/
        /*Debugging
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 0, 11, 'l' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 1, 11, ':' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 2, 11, ( tab->line ) + 48 );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 3, 11, ',' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 4, 11, 's' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 5, 11, ':' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 6, 11, tab->lines + 48 );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 7, 11, ',' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 8, 11, 'x' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 9, 11, ':' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 10, 11, tab->cursorX + 48 );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 11, 11, ',' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 12, 11, 'y' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 13, 11, ':' );
        interrupt( 0x21, INTERRUPT_21_PRINT_AT_LOCATION_M, 14, 11, tab->cursorY + 1 + 48 );
        */
    }
}

void SaveFile( struct EditorTab* tab )
{
    char writeFile[ FILE_NAME_SIZE_LIMIT_M + 3 ];
    char i = 2;
    //interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, "SAVING FILE", INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
    writeFile[ 0 ] = '\0';
    writeFile[ 1 ] = 255;
    writeFile[ 2 ] = tab->fileName[ 0 ];
    writeFile[ 3 ] = tab->fileName[ 1 ];
    writeFile[ 4 ] = tab->fileName[ 2 ];
    writeFile[ 5 ] = tab->fileName[ 3 ];
    writeFile[ 6 ] = tab->fileName[ 4 ];
    writeFile[ 7 ] = tab->fileName[ 5 ];
    writeFile[ 8 ] = '\0';
    //for( ; i < 9; ++i )
    //    writeFile[ i ] = tab->fileName[ i ];
    //interrupt( 0x21, INTERRUPT_21_PRINT_STRING_M, writeFile + 2, INTERRUPT_21_PRINT_STRING_PRINT_LINE_M, 0 );
    interrupt( 0x21, INTERRUPT_21_WRITE_FILE_M, writeFile, tab->text, 
                    Divide( ( tab->lines * tab->bufferLength ), SECTOR_SIZE_M ) + 1 );
}

void DetermineModeAndFileNameFromCommandLine( char* fileName, char* commandLineCreateCommandMode )
{
    char buffer[ COMMAND_LINE_ARGUMENT_BUFFER_LENGTH_M ];
    char* signal = TEXT_EDITOR_COMMAND_LINE_MODE_M;
    unsigned int i = 0;
    *commandLineCreateCommandMode = 1;
    interrupt( 0x21, INTERRUPT_21_COMMAND_LINE_ARGUMENT_GET_M, buffer, 0, 0 );
    for( ; i < TEXT_EDITOR_COMMAND_LINE_MODE_LENGTH_M; ++i )
    {
        if( signal[ i ] != buffer[ i ] ) {
            *commandLineCreateCommandMode = 0;
            break;
        }
    }
    if( *commandLineCreateCommandMode == 1 )
    {
        *commandLineCreateCommandMode = 1;
        for( ; i < ( TEXT_EDITOR_COMMAND_LINE_MODE_LENGTH_M + FILE_NAME_SIZE_LIMIT_M ); ++i )
            fileName[ i - TEXT_EDITOR_COMMAND_LINE_MODE_LENGTH_M ] = buffer[ i ];
    }
}
