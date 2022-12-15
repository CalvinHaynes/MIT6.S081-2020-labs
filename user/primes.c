#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void prime(int pL[2]) //输入左管道
{
    int p;
    close(pL[1]);
    if (read(pL[0],&p,4) == 0)//第一个读取的必定是质数
        return;
    printf("prime %d\n",p);

    int pR[2]; //创建右通道
    pipe(pR);

    int pid=fork();
    if (pid > 0) { //主进程
        close(pR[0]);
        int n;
        while (read(pL[0], &n, sizeof(n)))
        {
            if(n % p != 0) {
                write(pR[1], &n, sizeof(n));
            }
        }
        close(pR[1]);
        close(pL[0]);
        wait(0);
    }
    else if (pid == 0)//子进程
    {
        close(pL[0]);
        close(pR[1]);
        prime(pR);
        close(pR[0]);
    }
    exit(0);
}

int main(int argc, char *argv[]){
    int p[2]; //r、w
    pipe(p);

    if(fork() != 0)
    {
        close(p[0]);
        for (int i = 2; i <= 35; ++i)
            write(p[1],&i,4);
        close(p[1]);
        wait(0);
    }
    else
    {
        close(p[1]);
        prime(p);
        close(p[0]);
    }
    exit(0);
}
