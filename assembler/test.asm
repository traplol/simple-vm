li $g0, (1)       # start
li $g1, (5)       # end
li $g2, (1)       # product

loop:
eq $g0, $g1
jzs exit          # if g0 > g1, exit loop
mul $g2, $g1      # g2 = g2 * g1
addi $g1, (-1)    # g1 = g1 + 1
j loop           # continue loop

exit:
halt              # done.
