#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define STDIN_FD 0
#define INPUT_BUF_SIZE 1024
#define SPEED_MIN 0
static volatile sig_atomic_t g_inc_pending = 0;
static volatile sig_atomic_t g_dec_pending = 0;

static void print_speed_change(int is_increase, int speed)
{
    if (is_increase)
        printf("increase speed to: %d\n", speed);
    else
        printf("decrease speed to: %d\n", speed);

    fflush(stdout);
}

static void handle_sigusr1(int signo)
{
    (void)signo;
    g_dec_pending = 1;
}

static void handle_sigusr2(int signo)
{
    (void)signo;
    g_inc_pending = 1;
}

int main(void)
{
    int speed = 0;
    char buf[INPUT_BUF_SIZE];

    printf("PID=%d\n", (int)getpid());
    fflush(stdout);

    struct sigaction sa1;
    struct sigaction sa2;

    sigemptyset(&sa1.sa_mask);
    sa1.sa_flags = 0;
    sa1.sa_handler = handle_sigusr1;

    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;
    sa2.sa_handler = handle_sigusr2;

    if (sigaction(SIGUSR1, &sa1, NULL) != 0) {
        perror("sigaction SIGUSR1");
        return 1;
    }

    if (sigaction(SIGUSR2, &sa2, NULL) != 0) {
        perror("sigaction SIGUSR2");
        return 1;
    }

    for (;;) {
        while (g_inc_pending || g_dec_pending) {
            if (g_inc_pending) {
                g_inc_pending = 0;
                speed++;
                print_speed_change(1, speed);
            }

            if (g_dec_pending) {
                g_dec_pending = 0;
                if (speed > SPEED_MIN)
                    speed--;
                print_speed_change(0, speed);
            }
        }
        ssize_t nread = read(STDIN_FD, buf, sizeof(buf));

        if (nread < 0) {
            if (errno == EINTR)
                continue;
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
