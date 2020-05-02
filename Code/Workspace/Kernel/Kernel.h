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
#include "Process.h"

/*#define INTERRUPT_21_PRINT_STRING_M 0
#define INTERRUPT_21_PRINT_STRING_PRINT_STRING_M 0
#define INTERRUPT_21_PRINT_STRING_PRINT_LINE_M 1
#define INTERRUPT_21_PRINT_STRING_TERMINAL_PRINT_M 2
#define INTERRUPT_21_READ_STRING_FROM_KEYBOARD_M 1
#define INTERRUPT_21_READ_STRING_FROM_KEYBOARD_READ_DEFAULT_M 0
#define INTERRUPT_21_READ_STRING_FROM_KEYBOARD_READ_UNLIMITED 1
#define INTERRUPT_21_READ_STRING_FROM_KEYBOARD_READ_RANGE_M 2
#define INTERRUPT_21_READ_SECTOR_M 2
#define INTERRUPT_21_READ_SECTOR_DEFAULT_CALCULATE_RELATIVE_SECTOR_M 0
#define INTERRUPT_21_READ_SECTOR_DEFAULT_CALCULATE_HEAD_M 0
#define INTERRUPT_21_READ_SECTOR_DEFAULT_CALCULATE_TRACK_M 0
#define INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_M 3
#define INTERRUPT_21_SET_SECTOR_IO_DEVICE_NUMBER 9
#define INTERRUPT_21_SET_ALTERNATE_SECTOR_CHS_M 10
#define INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_TRUE_M 2
#define INTERRUPT_21_USE_ALTERNATE_SECTOR_CHS_FALSE_M 1
#define INTERRUPT_21_TERMINATE_M 5
/*#define INTERRUPT_21_GET_SECTOR_IO_DEVICE_NUMBER 11*/

void handleInterrupt21( int AX, int BX, int CX, int DX );

extern METHOD_READ_CALCULATOR_T alternateRelativeSectorCalculator;
extern METHOD_READ_CALCULATOR_T alternateHeadCalculator;
extern METHOD_READ_CALCULATOR_T alternateTrackCalculator;
extern int useAlternateCalculators;
extern int sectorIODeviceNumber;
