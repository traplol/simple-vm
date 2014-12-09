
li $g0, (1)
li $g1, (1)
li $g2, (5)

loop:
eq $g2, $g1
jzs exit
mul $g0, $g2
addi $g2, (-1)
js loop

exit:
halt


