
Instruction layout
==================
```
Bit Key:
o - Opcode
r - register
m - immediate value
0 - ignored
```
All valid instructions follow one of the following bit layouts
### No operands
```
oooooo 00000000000000000000000000
  6               26
```
These types of instructions take no operands.

*Note: The 26 lowest bits are ignored.*

### Register to register
```
oooooo rrrrr rrrrr 0000000000000000
  6      5     5          16
```
These types of instructions take two operands in the form of register identifiers and 
will have the value of the second register applied to the value of the first register
with the result stored in the first register.

*Note: The 16 lowest bits are ignored.*

### Register to register with an offset
```
oooooo rrrrr rrrrr mmmmmmmmmmmmmmmm
  6      5     5          16
```
These types of instructions take three operands in the form of two register identifiers
and an immediate 16-bit value. The value of the second register and the immediate should
be added together and then the result applied to the first register storing the result
in the first register.

*Note: The sign bit of the 16-bit immediate is preserved*

### Immediate to register
```
oooooo rrrrr mmmmmmmmmmmmmmmmmmmmm
  6      5            21
```
These types of instructions take two operands in the form of one register identifer and
one immediate 21-bit value. The immediate value should be applied to the value of the
register and the result stored in the register.

*Note: The sign bit of the 21-bit immediate is preserved*

### Single register with no immediate:
```
oooooo rrrrr 000000000000000000000
  6      5            21
```
These types of instructions take a single operand in the form of a register identifier.

*Note: The 21 lowest bits are ignored.*

### Immediate with no register:
```
oooooo 00000 mmmmmmmmmmmmmmmmmmmmm
  6      5            21
```
These types of instructions take a single operand in the form of an immediate value stored
in the 21 lowest bits.

*Note: Bits 22-26, inclusive, are ignored.*

*Note: The sign bit of the 21-bit immediate is preserved*

