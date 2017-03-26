# BreakVinzor

## Introduction
This program can simulate the behaviour of a PS2 keyboard by manipulating the I/O port on Windows Operation System.
My work is based on WinRing0, a powerful hardware access library for Windows.

This program also support typing a file to a remote desktop where network access is restricted.
(You know what I mean.) You can doing as follows:

1. **Encoding the file you want to transit.** That is, convert the binary file to a text file.
Regard each byte as an unsigned integer and represent it to hexadecimal format.
So, each byte will be converted into 2 ascii characters([0-9a-f]).

2. **Typing the hex ascii characters to the remote desktop.** Prepareing a blank text file and save it after receiving characters.

3. **Typing the source code(decoder.c) to the remote desktop.** Compile the source code.

4. **Decoding the hex ascii characters to get original file.**


## Direction
Open the solution Keyboard_Simulator.sln with Visual Stdio 2017. This solution include 3 project: Keyboard_Simulator, encoder and decoder.
You may need Keyboard_Simulator project only.

Generate the the project Keyboard_Simulator.

1.**Copy WinRing0.dll and WinRing0.sys (or WinRing0x64.dll and WinRing0x64.sys on x64 Operating System) to the directory
where the executable file, Keyboard_Simulator.exe, locates.** It is only dependent on whether your OS is x86 or x64
but the program! It is okay to copy all the four files.

2.**Run Keyboard_Simulator.exe as administrator**

3.**Type h[elp] to get help**
