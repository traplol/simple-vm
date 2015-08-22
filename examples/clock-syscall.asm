.data
.asciiz msg_beg: "It took "
.asciiz msg_end: " microseconds to count to "

.text
li $sys, (1)            # sys-1 is the clock function
syscall                 # get starting time
mov $g1, $r1

li $g8, (0)             # starting number
li $g9, (152)           # upper half of 10,000,000
li $g10, (16)
sll $g9, $g10           # shift by 16 to put upper half in place
li $g10, (38528)        # lower half of 10,000,000
or $g9, $g10            # or the two to get both halves in the register

# Simple "for" loop from 0-10,000,001
loop:
    gt $g8, $g9
    jzs done
    addi $g8, (1)
    js loop

done:
syscall                 # get end time, $sys is never changed so it's still clock
muli $g1, (-1)          # mul start time by -1 so we can subtract by adding
add $r1, $g1            # subtract by adding
mov $g1, $r1

li $g4, msg_beg
prints $g4

printi $g1              # verify exactly how many iterations it took.

li $g4, msg_end
prints $g4
printi $g8
printc '\n'
halt
