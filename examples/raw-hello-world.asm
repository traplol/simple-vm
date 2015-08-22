.text
    li $g0, array

    li $g1, (72)
    sb $g1, $g0, (0)        # 'H'
    li $g1, (101)
    sb $g1, $g0, (1)        # 'e'
    li $g1, (108)
    sb $g1, $g0, (2)        # 'l'
    sb $g1, $g0, (3)        # 'l'
    li $g1, (111)
    sb $g1, $g0, (4)        # 'o'

    li $g1, (32)
    sb $g1, $g0, (5)        # ' '

    li $g1, (87)
    sb $g1, $g0, (6)        # 'W'
    li $g1, (111)
    sb $g1, $g0, (7)        # 'o'
    li $g1, (114)
    sb $g1, $g0, (8)        # 'r'
    li $g1, (108)
    sb $g1, $g0, (9)        # 'l'
    li $g1, (100)
    sb $g1, $g0, (10)       # 'd'
    li $g1, (33)
    sb $g1, $g0, (11)       # '!'

    li $g1, (10)
    sb $g1, $g0, (12)       # <newline>

    prints $g0
    halt

.data

.raw array: (128)
