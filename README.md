simple-vm
=========
a very simple virtual machine and assembler.

simple-vm is a 32-bit register based virtual machine. It shares similarities to a MIPS 
architecture because it has a total of 23 registers and all of the instructions are a 
fixed width of 32 bits.

usage
=====
```
$ git clone https://github.com/traplol/simple-vm.git
$ cd simple-vm
$ make
$ cd bin
$ ./assembler ../examples/recursive.asm -o recursive.bin
$ ./vm recursive.bin
120
```

docs
====

[Binary file format](docs/binary-file-format.md)

[Instruction layout](docs/instruction-layout.md)

[Instructions](doc/instructions.md)