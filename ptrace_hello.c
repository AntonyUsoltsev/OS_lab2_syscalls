//#include <sys/ptrace.h>
//#include <unistd.h>
//
//
//void child() {
//    ptrace(PTRACE_TRACEME, 0, 0, 0);
//    execl("/bin/echo", "/bin/echo", "Hello, world!", NULL);
//    perror("execl");
//}
//
//
//int main(int argc, char *argv[]) {
//    pid_t pid = fork();
//    if (pid)
//        parent(pid);
//    else
//        child();
//    return 0;
//}

//#include <sys/ptrace.h>
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <unistd.h>
//#include <stdio.h>
//#include <linux/user.h>   /* For constants
//                                   ORIG_EAX etc */
//
//
//void child() {
//    ptrace(PTRACE_TRACEME, 0, 0, 0);
//    execl("/bin/echo", "/bin/echo", "Hello, world!", NULL);
//    perror("execl");
//}
//
//int main() {
//    pid_t pid = fork();
//    if (pid)
//        parent(pid);
//    else
//        child();
//    if (child == 0) {
//        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
//        execl("D:/Antony/NSU_Education/4_Term/OS/lab2_syscalls/main", "ls", NULL);
//    } else {
//        wait(NULL);
//        orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
//        printf("The child made a system call %ld\n", orig_eax);
//        ptrace(PTRACE_CONT, child, NULL, NULL);
//    }
//
//    int status;
//    waitpid(pid, &status, 0);
//
//    while (!WIFEXITED(status)) {
//
//        struct user_regs_struct state;
//
//        ptrace(PTRACE_SYSCALL, pid, 0, 0);
//        waitpid(pid, &status, 0);
//
//        // at syscall
//        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
//            ptrace(PTRACE_GETREGS, pid, 0, &state);
//            printf("SYSCALL %d at %08lx\n", state.orig_eax, state.eip);
//
//            // skip after syscall
//            ptrace(PTRACE_SYSCALL, pid, 0, 0);
//            waitpid(pid, &status, 0);
//        }
//
//    }
//    return 0;
//}



#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>

int main(int argc, char *argv[]) {
    pid_t child_pid;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s program [args...]\n", argv[0]);
        exit(1);
    }
    child_pid = fork();
    if (child_pid == 0) {
        // Child process
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(1);
    } else if (child_pid > 0) {
        // Parent process
        int status;
        waitpid(child_pid, &status, 0);

        ptrace(PTRACE_SETOPTIONS, child_pid, NULL, PTRACE_O_TRACESYSGOOD);

        while (WIFSTOPPED(status)) {
            struct user_regs_struct regs;
            ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
        //    puts("hi");
            waitpid(child_pid, &status, 0);

            if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
                ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
                printf("System call %lld\n", regs.orig_rax);
            }
        }
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}

//Here's how the program works:
//
//The program first checks that the user has provided a command to run as a child process.

//The program forks a new process, which becomes the child process.
// The child process first calls ptrace(PTRACE_TRACEME, 0, NULL, NULL) to allow its parent process to trace it, and then
// calls execvp() to replace its own image with the specified command.

//The parent process waits for the child process to stop and then calls ptrace(PTRACE_SETOPTIONS, child_pid, NULL, PTRACE_O_TRACESYSGOOD)
// to enable system call tracing.

//The parent process enters a loop where it calls ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL)
// to continue the child process's execution and waits for the child process to stop again.
//If the child process was stopped due to a system call, the parent process prints out the system call
// number using ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) to get the register values of the child process.
//Note that the PTRACE_O_TRACESYSGOOD option is used in ptrace(PTRACE_SETOPTIONS) to ensure that the parent process is
// notified when a system call is made by the child process, as opposed to other types of events that could also trigger
// a stop. This option is only available starting with Linux kernel version 2.6.18.


//?????? ?????? ???????????????? ??????????????????:
//
//?????????????? ?????????????????? ??????????????????, ?????????????????????? ???? ???????????????????????? ?????????????? ?????? ?????????????? ?? ???????????????? ?????????????????? ????????????????.
//?????????????????? ?????????????????????? ?????????? ??????????????, ?????????????? ???????????????????? ???????????????? ??????????????????. ???????????????? ?????????????? ?????????????? ????????????????
// ptrace(PTRACE_TRACEME, 0, NULL, NULL), ?????????? ?????????????????? ?????????????????????????? ???????????????? ?????????????????????? ??????, ?? ?????????? ????????????????
// execvp() ?????? ???????????? ???????????????????????? ?????????????????????? ?????????????????? ????????????????.
//???????????????????????? ?????????????? ?????????????? ?????????????????? ?????????????????? ????????????????, ?? ?????????? ????????????????
// ptrace(PTRACE_SETOPTIONS, child_pid, NULL, PTRACE_O_TRACESYSGOOD) ?????? ?????????????????? ?????????????????????? ?????????????????? ??????????????.
//???????????????????????? ?????????????? ???????????? ?? ????????, ?? ?????????????? ???? ???????????????? ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) ?????? ??????????????????????
// ???????????????????? ?????????????????? ???????????????? ?? ????????, ???????? ???????????????? ?????????????? ?????????? ??????????????????????.
//???????? ???????????????? ?????????????? ?????? ???????????????????? ????-???? ???????????????????? ????????????, ???????????????????????? ?????????????? ?????????????? ?????????? ???????????????????? ????????????,
// ?????????????????? ptrace(PTRACE_GETREGS, child_pid, NULL, &regs), ?????????? ???????????????? ???????????????? ?????????????????? ?????????????????? ????????????????.
//???????????????? ????????????????, ?????? ???????????????? PTRACE_O_TRACESYSGOOD ???????????????????????? ?? ptrace(PTRACE_SETOPTIONS), ?????????? ??????????????????,
// ?????? ???????????????????????? ?????????????? ????????????????????????, ?????????? ???????????????? ?????????????? ?????????????????? ?????????????????? ??????????, ?? ?????????????? ???? ???????????? ?????????? ??????????????,
// ?????????????? ?????????? ?????????? ?????????????? ??????????????????. ?????? ?????????? ???????????????? ???????????? ?????????????? ?? ???????????? ???????? Linux 2.6.18.