.ent printHelloWorld
.text
.global printHelloWorld
printHelloWorld:
addi $sp, $sp, -12
sw $t0, 0($sp)
sw $a0, 4($sp)
sw $a1, 8($sp)

lw $t0, count
addi $t0, $t0, 1
sw $t0, count
la $a0, myStr
lw $a1, count
jal printf
nop

lw $t0, 0($sp)
lw $a0, 4($sp)
lw $a1, 8($sp)
addi $sp, $sp, 12
.data
count: .word 0
myStr: .asciiz "Hello, world! %d"
.end printHelloWorld