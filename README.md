## BreakVinzor

#Introduction
This program can simulate the behaviour of the PS2 keyboard by manipulating the I/O port on Windows Operation System.
My work is based on WinRing0, a powerful hardware access library for Windows.

This program alse support typing a file to a remote desktop where network access is restricted.
(You know what I mean.) You can doing as follows:

1. **Encoding the file you want to transit.** That is, convert the binary file to a text file.
Regard each byte as an unsigned integer and represent it to hexadecimal format.
So, each byte will be converted into 2 ascii characters([0-9a-f]).

2. **Typing the hex ascii characters to the remote desktop.** Opening a blank file and save it after typing.

3. **Typing the source code(decoder.c) to the remote desktop** Compilde the source code.

4. **Decode the hex ascii characters to get original file.**


#Direction
Open the solution Keyboard_Simulator.sln. This solution include 3 project: Keyboard_Simulator, encoder and decoder.
You probably need Keyboard_Simulator project only.

Compile the the project Keyboard_Simulator.

1.**Copy WinRing0.dll and WinRing0.sys (or WinRing0x64.dll and WinRing0x64.sys on x64 Operating System) to the directory
where the executable file, Keyboard_Simulator.exe, locates.** It is dependent on whether your OS is x86 or x64
not your program! It is okay to copy all the four files.

2.**Run Keyboard_Simulator.exe as administrator**

3.**Type h[elp] to get help**
