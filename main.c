#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <asm/unistd.h>
#include <asm/unistd_64.h>
#include <syscall.h>
#include <string.h>


int main() {
    printf("Hello, %s!\n", "Anton");

//    write(1, "Hello, World!\n", 14);
//    // file descriptor 0-stdin, 1-stdout, 2-stderr
//    syscall(SYS_write, 1, "Hello, World!\n", 14);
//
//    char *hello = "Hello, World! Asm\n";
//    unsigned long len = strlen(hello);
//    int ret = 0;
//    asm("mov rax, 1; \n\t"
//        "mov rdi, 1; \n\t"
//        "mov rsi, %1; \n\t"
//        "mov rdx, %2; \n\t"
//        "syscall"
//    : "=g"(ret)
//    : "g"(hello), "g"(len));

    return 0;
}