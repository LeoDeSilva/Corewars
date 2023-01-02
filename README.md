# Corewars
Corewars is an old 80's programming game where you write assembly 'Warriors' to 'take over' the memory of a computer, 2 programs are loaded into random addresses in memory and through a [series of instructions](https://vyznev.net/corewar/guide.html) compete to eradicate each other and gain controll of memory. 
Each program takes it in turns to execute a single command and a program fails if it attempts to execute an invalid instruction (e.g. 0x00)

## Installation & Useage
Clone repository with `git clone`, example programs should be listed in `warriors` or `Emulator` for pre-compiled alternatives. 
To compile a warrior (.asm endings in `warrior` directory), enter the `Assembler` directory and run `./assembler <prog>.asm <out>.bin` (if no output file is specified, it will default to `out.bin`)
A binary `.bin` should be produced which can be used as inputs to the main emulator program.

To simulate the game, enter the `Emulator` directory and run './main <prog1>.bin <prog2.bin>`. 

e.g. `./main dwarf.bin gun.bin` should produce this (programms pre-compiled in the Emulator directory)

Example `gun.asm` program: 
```asm 
djn 7, 2
jmp 6
mov 4, 1337
spl @-1
add #1337, -2
jmp -5
mov 0, 1
dat #0, #5
```
![CleanShot 2023-01-02 at 15 38 23@2x](https://user-images.githubusercontent.com/46300158/210252746-f0bd3c48-9d5a-4794-9f51-7a1bec438913.png)

## Redcode Cheatsheet
RedCode is composed of a series of 16 instructions that vary from traditional asm instrucitons e.g. `mov`, `add`, etc... to instructions that split the program into two paralell threads (used in `gun.bin`)
Here is a quick overview of the instructions, however [Corewar](https://corewar.co.uk/guides.htm) offers a much more comprehensive guide.
- `mov`
- `dat`
- `add`
- `sub`
- `mul` 
- `div`
- `mod`
- `jmp`
- `jmz`
- `jmn`
- `djn`
- `spl`
- `slt`
- `seq`
- `sne`
- `nop`
