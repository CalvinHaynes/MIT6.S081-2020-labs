#include "kernel/types.h"
#include "user/user.h"

char ping[5] = "ping";
char pong[5] = "pong";

int main(int argc, char *argv[])
{
    int pid;
    // fd[0] read, fd[1] write 
    int parent_fd[2];   // parent send, child receive
    int child_fd[2];    // child send, parent receive
    pipe(parent_fd);
    pipe(child_fd);

    char buf[5];

    if ((pid = fork()) != 0) {
        // father
        close(parent_fd[0]);
        close(child_fd[1]);
        write(parent_fd[1], ping, sizeof ping);
        read(child_fd[0], buf, sizeof buf);
        printf("%d: received pong\n", getpid());
    } else {
        // child
        close(child_fd[0]);
        close(parent_fd[1]);
        read(parent_fd[0], buf, sizeof buf);
        printf("%d: received ping\n", getpid());
        write(child_fd[1], pong, sizeof pong);
    }

    exit(0);
}