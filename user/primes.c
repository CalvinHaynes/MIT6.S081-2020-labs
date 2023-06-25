#include "kernel/types.h"
#include "user/user.h"

const int end = -1;

__attribute__((noreturn))
void prime_handler(int receive_pipe)
{
    int buf;
    int pid;
    int idx = 0;
    int pos_prime;
    int send_pipe[2];
    int to_process[35];
    int create_child = 0;

    memset(to_process, 0, sizeof to_process);

    read(receive_pipe, &buf, sizeof(int));
    while (buf != -1) {
        to_process[idx++] = buf;
        read(receive_pipe, &buf, sizeof(int));
    }
    close(receive_pipe);

    int len = idx;

    for (int i = 0; i < len; i++) {
        if (i == 0) {
            pos_prime = to_process[i];
            printf("prime %d\n", pos_prime);
            continue;
        }

        if (to_process[i] % pos_prime != 0) {
            if (!create_child) {
                create_child = 1;
                pipe(send_pipe);
                if ((pid = fork()) == 0) {
                    // child
                    close(send_pipe[1]);
                    prime_handler(send_pipe[0]);
                }
            }
            write(send_pipe[1], &to_process[i], sizeof(int));
        }
    }

    if (create_child) {
        write(send_pipe[1], &end, sizeof(int));
        close(send_pipe[1]);
        wait(&pid);
    }

    exit(0);
}

int main(int argc, char *argv[])
{
    int pid;
    int send_pipe[2];
    pipe(send_pipe);

    if ((pid = fork()) == 0) {
        // child
        close(send_pipe[1]);
        prime_handler(send_pipe[0]);
    }

    // root
    int pos_prime = 2;
    close(send_pipe[0]);

    for (int i = 2; i <= 35; i++) {
        if (i == pos_prime) {
            printf("prime %d\n", pos_prime);
            continue;
        }

        if (i % pos_prime != 0) {
            write(send_pipe[1], &i, sizeof(int));
        }
    }
    write(send_pipe[1], &end, sizeof(int));
    close(send_pipe[1]);
    wait(&pid);

    exit(0);
}