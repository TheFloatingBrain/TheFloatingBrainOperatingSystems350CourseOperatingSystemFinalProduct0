#!/bin/bash

function DebugEcho()
{
	if [[ $2 == '-debug' ]] || [[ $3 == '-debug' ]] || [[ $2 == '--debug' ]] || [[ $3 == '--debug' ]] || [[ $2 == '--dbg' ]] || [[ $3 == '--dbg' ]]
	then
		echo "Debug: " $1
	fi
}

if [ $1 ]
then
	if [ $1 == '-clean' ] || [ $1 == '--clean' ] || [ $1 == '-cln' ]
	then
			DebugEcho "Cleaning" $1 $2
			./Clean.sh
			DebugEcho "Done Cleaning" $1 $2
	elif [ $2 ]
	then
		if [ $2 == '-clean' ] || [ $2 == '--clean' ] || [ $2 == '-cln' ]
		then
			DebugEcho "Cleaning" $1 $2
			./Clean.sh
			DebugEcho "Done Cleaning" $1 $2
		fi
	fi
fi


DebugEcho "Build Started"
DebugEcho "Resetting FloppyA" $1 $2
./ResetFloppyA.sh
DebugEcho "Done Resetting FloppyA" $1 $2
DebugEcho "Assembling Bootloader" $1 $2
./AssembleBootloader.sh
DebugEcho "Done Assembling Bootloader" $1 $2
DebugEcho "Building Kernel Library Assembly" $1 $2
./BuildKernelLibAsm.sh
DebugEcho "Done Building Kernel Library Assembly" $1 $2
DebugEcho "Building Kernel C Support Library" $1 $2
./CompileLibrary.sh
DebugEcho "Building Kernel C Support Library" $1 $2
DebugEcho "Building C Kernel" $1 $2
./BuildCKernel.sh
DebugEcho "Done Building C Kernel" $1 $2
DebugEcho "Linking Kernel" $1 $2
./LinkKernel.sh
DebugEcho "Done Linking Kernel" $1 $2
DebugEcho "Writing Bootloader To Floppy A" $1 $2
./WriteBootloaderToFloppyA.sh
DebugEcho "Done Writing Bootloader To Floppy A" $1 $2
DebugEcho "Writing Kernel To Floppy A" $1 $2
./WriteKernelToFloppyA.sh
DebugEcho "Done Writing Kernel To Floppy A" $1 $2
DebugEcho "Building Shell" $1 $2
./BuildCShell.sh
DebugEcho "Done Building Shell" $1 $2
DebugEcho "Linking Shell" $1 $2
./LinkShell.sh
DebugEcho "Done Linking Shell" $1 $2
DebugEcho "Building Text Editor" $1 $2
./BuildTextEditor.sh
DebugEcho "Done Building Text Editor" $1 $2
DebugEcho "Linking Text Editor" $1 $2
./LinkTextEditor.sh
DebugEcho "Done Linking Text Editor" $1 $2
DebugEcho "Writing Shell To Floppy A" $1 $2
./WriteShellToFloppyA.sh
DebugEcho "Done Writing Shell To Floppy A" $1 $2
DebugEcho "Writing Text Editor To Floppy A" $1 $2
./WriteTextEditorToFloppyA.sh
DebugEcho "Done Writing Text Editor To Floppy A" $1 $2
DebugEcho "Writing Directory To Floppy A" $1 $2
./WriteDirectoryToFloppyA.sh
DebugEcho "Done Writing Directory To Floppy A" $1 $2
DebugEcho "Writing Map To Floppy A" $1 $2
./WriteMapToFloppyA.sh
DebugEcho "Done Writing Map To Floppy A" $1 $2
DebugEcho "Writing Test Files To Floppy A" $1 $2
./WriteTestFilesToFloppyA.sh
DebugEcho "Done Writing Test Files To Floppy A" $1 $2
DebugEcho "Build Done!"
