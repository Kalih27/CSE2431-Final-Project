/*
** test.c for lkm-syscall
**
** Originally made by xsyann
** Contact <contact@xsyann.com>
**
** Current version built by Yuan Xiao
** Contact <xiao.465@osu.edu>
*/

#include <sys/syscall.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define HIJACKED_SYSCALL __NR_tuxcall

char buffer[40];

int foo = 43;

int main(int argc, char* argv[])
{
        // char *string;

        printf("Call syscall at offset %d\n", HIJACKED_SYSCALL);
        // string = argc > 1 ? argv[1] : "Foo";
        printf("Virtual address @%p\n", &foo);
        snprintf(buffer, sizeof(buffer), "%lx", (size_t)(&foo));
        printf("Syscall return %lx\n", syscall(HIJACKED_SYSCALL, buffer));
        return 0;
}
