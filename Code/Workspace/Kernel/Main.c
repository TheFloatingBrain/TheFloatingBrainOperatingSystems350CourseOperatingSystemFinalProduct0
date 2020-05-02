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

main()
{
	char shell[ 6 ];
	makeInterrupt21();
	shell[ 0 ] = 'S';
	shell[ 1 ] = 'h';
	shell[ 2 ] = 'e';
	shell[ 3 ] = 'l';
	shell[ 4 ] = 'l';
	shell[ 5 ] = '\0';
	ExecuteProgram( shell, 0x2000 );
	while( 1 );
}

/*Project C
main()
{
	char shell[ 6 ];
	makeInterrupt21();
	shell[ 0 ] = 'S';
	shell[ 1 ] = 'h';
	shell[ 2 ] = 'e';
	shell[ 3 ] = 'l';
	shell[ 4 ] = 'l';
	shell[ 5 ] = '\0';
	ExecuteProgram( shell, 0x2000 );
	while( 1 );
}
*/

/*Before terminate run program.
main()
{
	char buffer[ 512 * 4 ];
	makeInterrupt21();
	PrintLine( "Starting TSTPR2" );
	StartProgram( "TSTPR2", 0x2000 );
	PrintLine( "Done" );
	while( 1 );
}
*/

/*Read a simple file.
main()
{
	char buffer[ 512 * 2 ];
	int found = 0;
	makeInterrupt21();
	PrintLine( "Reading Message!" );
	found = ReadFile( "MESSAG", buffer );
	if( found == 0 )
		PrintLine( "Did not find file." );
	else {
		PrintLine( "Found file!" );
		PrintLine( buffer );
	}
	PrintLine( "Done" );
	while( 1 );
}*/
