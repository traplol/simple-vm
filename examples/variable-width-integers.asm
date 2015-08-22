.text
    # 8-bit numbers
    li $g0, test_i8
    lb $g0, $g0, $nul
    printi $g0
    printc (10)

    # 16-bit numbers
    li $g0, test_i16
    lb $g1, $g0, (1) # upper
    li $g2, (8)
    sll $g1, $g2     # shift upper into place
    lb $g2, $g0, (0) # lower
    or $g1, $g2
    printi $g1
    printc (10)

    # 32-bit numbers
    li $g0, test_i32
    lw $g1, $g0, $nul
    printi $g1
    printc (10)
    
    halt

.data

.i8 test_i8: (123)
.i16 test_i16: (54321)
.i32 test_i32: (123456789)

.i8 i8_max: (255)
.i16 i16_max: (65535)
.i32 i32_max: (4294967295)
