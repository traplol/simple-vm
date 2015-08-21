.text
li $g0, hello_world
prints $g0

li $g0, test_1
prints $g0

li $g0, test_2
prints $g0

li $g0, test_3
prints $g0

li $g0, test_4
prints $g0

li $g0, test_5
prints $g0

halt

.data
.asciiz hello_world: "Hello world\n"

.asciiz test_1: "tab\t\tab\t\ttab\n"
.asciiz test_2: ""
.asciiz test_3: "\\\\\\\\\\\n"
.asciiz test_4: "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
.asciiz test_5: "\"simple-vm\"\n"
