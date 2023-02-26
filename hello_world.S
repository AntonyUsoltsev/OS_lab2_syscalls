#include <asm/unistd.h>
#include <syscall.h>

.data

msg:
   .ascii "Hello, world, ASM!\n"
   len = . - msg

.text
    .global _start
_start:
    movq $SYS_write, %rax
    movq $1, %rdi
    movq $msg, %rsi
    movq $len, %rdx
    syscall

#EXIT:
    movq $60, %rax
    xorq %rdi, %rdi
    syscall