.text
    li $g0, array

    li $g1, 'H'
    sb $g1, $g0, (0)        # 'H'
    li $g1, 'e'
    sb $g1, $g0, (1)        # 'e'
    li $g1, 'l'
    sb $g1, $g0, (2)        # 'l'
    sb $g1, $g0, (3)        # 'l'
    li $g1, 'o'
    sb $g1, $g0, (4)        # 'o'

    li $g1, ' '
    sb $g1, $g0, (5)        # ' '

    li $g1, '\x57'
    sb $g1, $g0, (6)        # 'W'
    li $g1, '\x6F'
    sb $g1, $g0, (7)        # 'o'
    li $g1, '\x72'
    sb $g1, $g0, (8)        # 'r'
    li $g1, '\x6c'
    sb $g1, $g0, (9)        # 'l'
    li $g1, '\x64'
    sb $g1, $g0, (10)       # 'd'
    li $g1, '\x21'
    sb $g1, $g0, (11)       # '!'

    li $g1, '\n'
    sb $g1, $g0, (12)       # <newline>

    prints $g0
    halt

.data

.raw array: (128)
