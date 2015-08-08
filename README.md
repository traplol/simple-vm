simple-vm
=========
a very simple virtual machine and assembler.

simple-vm is a 32-bit register based virtual machine. It shares similarities to a MIPS architecture because it has a total of 23 registers and all of the instructions are a fixed width of 32 bits.

instruction layout
==================

```
Bit Key:
o - Opcode
r - register
m - immediate value
0 - ignored
```
```
No operands:
oooooo 00000000000000000000000000
  6               26
```
```
Register to register:
oooooo rrrrr rrrrr 0000000000000000
  6      5     5          16
```
```
Register to register with an offset:
oooooo rrrrr rrrrr mmmmmmmmmmmmmmmm
  6      5     5          16
```
```
Immediate to register:
oooooo rrrrr mmmmmmmmmmmmmmmmmmmmm
  6      5            21
```
```
Single register with no immediate:
oooooo rrrrr 000000000000000000000
  6      5            21
```
```
Immediate with no register:
oooooo 00000 mmmmmmmmmmmmmmmmmmmmm
  6      5            21
```


