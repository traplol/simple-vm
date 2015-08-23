Instructions
============

* [No operands](#no-operands)
  * [HALT](#halt)
  * [NOP](#nop)
  * [RET](#ret)
  * [SYSCALL](#syscall)
* [Register to register](#register-to-register)
  * [ADD](#add)
  * [MUL](#mul)
  * [DIV](#div)
  * [EQ](#eq)
  * [NE](#ne)
  * [LT](#lt)
  * [LE](#le)
  * [GT](#gt)
  * [GE](#ge)
  * [AND](#and)
  * [OR](#or)
  * [XOR](#xor)
  * [SLL](#sll)
  * [SRL](#srl)
  * [MOV](#mov)
* [Register to register with an offset](#register-to-register-with-an-offset)
  * [LW](#lw)
  * [SW](#sw)
  * [LB](#lb)
  * [SB](#sb)
* [Immediate to register](#immediate-to-register)
  * [ADDI](#addi)
  * [MULI](#muli)
  * [DIVI](#divi)
  * [LI](#li)
* [Single register with no immediate](#single-register-with-no-immediate)
  * [JR](#jr)
  * [PUSH](#push)
  * [POP](#pop)
  * [PRINTS](#prints)
  * [PRINTI](#printi)
* [Immediate with no register](#immediate-with-no-register)
  * [J](#j)
  * [JS](#js)
  * [JZ](#jz)
  * [JZS](#jzs)
  * [CALL](#call)
  * [PUSHI](#pushi)
  * [PRINTC](#printc)


## No operands

#### HALT 
```
halt
```
Halt; stop and never resume program execution.

#### NOP 
```
nop
```
No-operation; does nothing but advance the ```PC```.

#### RET 
```
ret
```
Return; pop stack and set ```PC``` to value at top of stack.
Does not advance the ```PC```

#### SYSCALL 
```
syscall
```
System call; calls an internal function based on the value of the ```SYS``` register and stores the returned value in ```R1```.
Advances the ```PC```.

## Register to register

#### ADD 
```
add $r1, $r2
```
Add; stores the sum of $r1 and $r2 in $r1.
Advances the ```PC```.

#### MUL 
```
mul $r1, $r2
```
Multiply; stores the product of $r1 and $r2 in $r1.
Advances the ```PC```.

#### DIV 
```
div $r1, $r2
```
Divide; stores the quotient of $r1 divided by $r2 in $r1.
Advances the ```PC```.

#### EQ 
```
eq $r1, $r2
```
Equal; stores a 1 in ```Z``` if $r1 is equal to $r2, otherwise stores a 0.
Advances the ```PC```.

#### NE 
```
ne $r1, $r2
```
Not equal; stores a 1 in ```Z``` if $r1 is not equal to $r2, otherwise stores a 0.
Advances the ```PC```.

#### LT 
```
lt $r1, $r2
```
Less than; stores a 1 in ```Z``` if $r1 is less than $r2, otherwise stores a 0.
Advances the ```PC```.

#### LE 
```
le $r1, $r2
```
Less than or equal; stores a 1 in ```Z``` if $r1 is less than or equal to $r2, otherwise stores a 0.
Advances the ```PC```.

#### GT 
```
gt $r1, $r2
```
Greater than; stores a 1 in ```Z``` if $r1 is greater than $r2, otherwise stores a 0.
Advances the ```PC```.

#### GE 
```
ge $r1, $r2
```
Greater than or equal; stores a 1 in ```Z``` if $r1 is greater than or equal to $r2, otherwise stores a 0.
Advances the ```PC```.

#### AND 
```
and $r1, $r2
```
Logical AND; stores the result of a logical AND of $r1 and $r2 to $r1.
Advances the ```PC```.

#### OR 
```
or $r1, $r2
```
Logical OR; stores the result of a logical OR of $r1 and $r2 to $r1.
Advances the ```PC```.

#### XOR 
```
xor $r1, $r2
```
Logical XOR; stores the result of a logical XOR of $r1 and $r2 to $r1.
Advances the ```PC```.

#### SLL 
```
sll $r1, $r2
```
Logical shift left; stores the result of a logical shift left of $r1 by $r2 bits to $r1.
Advances the ```PC```.

#### SRL 
```
srl $r1, $r2
```
Logical shift right; stores the result of a logical shift right of $r1 by $r2 bits to $r1.
Advances the ```PC```.

#### MOV 
```
mov $r1, $r2
```
Move/copy; copies the value of $r2 to $r1.
Advances the ```PC```.

## Register to register with an offset

#### LW 
```
lw $r1, $r2, offset
```
Load word; loads the 32-bit value from memory address $r2 + offset into $r1.
Advances the ```PC```.

#### SW 
```
sw $r1, $r2, offset
```
Store word; stores the 32-bit value of $r1 into memory address $r2 + offset.
Advances the ```PC```.

#### LB 
```
lb $r1, $r2, offset
```
Load byte; loads the 8-bit value from memory address $r2 + offset into $r1.
Advances the ```PC```.

#### SB 
```
sb $r1, $r2, offset
```
Store byte; stores the 8-bit value of $r1 into memory address $r2 + offset.
Advances the ```PC```.

## Immediate to register

* Note: ```immediate``` in this context can be a label as labels just represent an address inserted by the assembler.

#### ADDI 
```
addi $r1, immediate
```
Add immediate; stores sum of $r1 and immediate in $r1.
Advances the ```PC```

#### MULI 
```
muli $r1, immediate
```
Multiply by immediate; stores the product of $r1 and immediate in $r1.
Advances the ```PC```

#### DIVI 
```
divi $r1, immediate
```
Divide by immediate; stores the quotient of $r1 divided by immediate in $r1.
Advances the ```PC```

#### LI 
```
li $r1, immediate
```
Load immediate; stores the immediate value in $r1.
Advances the ```PC```

## Single register with no immediate

#### JR 
```
jr $r1
```
Jump to register; sets ```PC``` to $r1
Does not advance the ```PC```

#### PUSH 
```
push $r1
```
Push register; pushes $r1 to the top of the stack and advances ```SP```.
Advances the ```PC```

#### POP 
```
pop $r1
```
Pop to register; decrements ```SP``` and stores the top of the stack in $r1.
Advances the ```PC```

#### PRINTS
```
prints $r1
```
Print string; prints a null-terminated string at the address in $r1.
Advances the ```PC```

#### PRINTI 
```
printi $r1
```
Print integer; prints the value of $r1.
Advances the ```PC```

## Immediate with no register
* Note: ```immediate``` in this context can be a label as labels just represent an address inserted by the assembler.

#### J 
```
j immediate
```
Jump; sets ```PC``` to the immediate value.
Does not advance the ```PC```.

#### JS 
```
js immediate
```
Short jump; adds the immediate value to ```PC```.
Does not advance the ```PC```.

#### JZ 
```
jz immediate
```
Jump if not zero; if ```Z``` is not zero set ```PC``` to immediate and set ```Z``` to zero.
Advances the ```PC``` only if ```Z``` is zero.

#### JZS 
```
jzs immediate
```
Short jump if not zero; if ```Z``` is not zero add immediate to ```PC``` and set ```Z``` to zero.
Advances the ```PC``` only if ```Z``` is zero.

#### CALL 
```
call immediate
```
Call; pushes the address of the next instruction to the top of the stack and advances ```SP```, then sets ```PC``` to immediate.
Does not advance ```PC```

#### PUSHI 
```
pushi immediate
```
Push immediate; pushes the immediate to the top of the stack and advances ```SP```.
Advances the ```PC```

#### PRINTC 
```
printc immediate
```
Print character; prints the immediate as a character.
Advances the ```PC```
