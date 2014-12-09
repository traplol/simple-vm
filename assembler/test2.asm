
li $g0, (42)
li $g1, (676)

pushi (5)
pushi (3)

call _add
j exit

_add:    

    add $g0, $g1
    mov $r0, $g0
    addi $fp, (-1)
    jr $fp



exit:
li $g9, (255)
halt
