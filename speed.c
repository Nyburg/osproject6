#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define STDIN_FD 0
#define INPUT_BUF_SIZE 1024
#define SPEED_MIN 0

static void print_speed_change(int is_increase, int speed)
{
    if (is_increase)
        printf("increase speed to: %d\n", speed);
    else
        printf("decrease speed to: %d\n", speed);

    fflush(stdout);
}

int main(void)
{
    int speed = 0;
    char buf[INPUT_BUF_SIZE];

    printf("PID=%d\n", (int)getpid());
    fflush(stdout);

    for (;;) {
        ssize_t nread = read(STDIN_FD, buf, sizeof(buf));

        if (nread < 0) {
            perror("read");
            return 1;
        }

        if (nread == 0)
            continue;

        for (ssize_t i = 0; i < nread; i++) {
            if (buf[i] == '+') {
                speed++;
                print_speed_change(1, speed);
            }
            else if (buf[i] == '-') {
                if (speed > SPEED_MIN)
                    speed--;
                print_speed_change(0, speed);
            }
            else if (buf[i] == 'q') {
                return 0;
            }
        }
    }
}