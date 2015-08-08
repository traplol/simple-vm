li $g0, (5)
call fact
j exit

fact:
    push $g0
    li $r0, (1)
    le $g0, $nul
    jzs fact_return
    addi $g0, (-1)
    call fact
    lw $g0, $sp, (4)
    mul $r0, $g0
fact_return:
    addi $sp, (4)
    ret

exit:
    printi $r0              # print the result
    halt
