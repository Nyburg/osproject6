#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("PID=%d\n", (int)getpid());
    return 0;
}