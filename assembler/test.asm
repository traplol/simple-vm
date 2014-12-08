li $g0, (1*1)     # start
li $g1, (1+2+2)   # end
li $g2, (-1*-1)   # product

loop:
gt $g0, $g1
jzs (4) #exit     # if g0 > g1, exit loop
mul $g2, $g1      # g2 = g2 * g1
addi $g1, (1/1)     # g1 = g1 + 1
js (-4) #loop     # continue loop

exit:
halt              # exit
