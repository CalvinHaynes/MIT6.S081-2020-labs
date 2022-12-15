#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p_p2c[2]; //读、写, 父进程 -> 子进程
    int p_c2p[2]; //子进程 -> 父进程
    pipe(p_p2c);
    pipe(p_c2p);
    if (fork() == 0){
        close(p_p2c[1]);
        close(p_c2p[0]);
        char *buffer[1];
        read(p_p2c[0],buffer,1);
        printf("%d: received ping\n", getpid());
        write(p_c2p[1],buffer,1); //发给父进程
        close(p_p2c[0]);
        close(p_c2p[1]);
        exit(0);
    }
    else
    {
        close(p_p2c[0]);
        close(p_c2p[1]);
        write(p_p2c[1],"1",1); //发给子进程
        wait(0);
        char *buffer[1];
        read(p_c2p[0],buffer,1);
        printf("%d: received pong\n", getpid());
        close(p_p2c[1]);
        close(p_c2p[0]);
    }
    exit(0);
}
