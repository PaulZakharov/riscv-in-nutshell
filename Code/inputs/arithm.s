.section .text
.globl _start
_start:
    addi sp, sp, -32
    li s2, 5
    addi s1, s2, 2
    slti s1, s2, -1
    sltiu s1, s2, -1
    li s2, 0xf000ffff
    xori s1, s2, 0xfffffaaa
    ori s1, s2, 0xfffffaaa
    andi s1, s2, 0xfffffaaa
    slli s1, s2, 4
    srai s1, s2, 4
    srli s1, s2, 4
    li s2, 0xf0ff0000
    li s3, 6
    add s1, s2, s3
    sub s1, s2, s3
    sll s1, s2, s3
    sra s1, s2, s3
    srl s1, s2, s3
    slt s1, s2, s3
    sltu s1, s2, s3
    li s2, 0x00ffff00
    li s3, 0xf0f0f0f0
    and s1, s2, s3
    or s1, s2, s3
    xor s1, s2, s3