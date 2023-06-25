#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define stdin 0
#define BUF_MAX 1024

char buf[BUF_MAX];
char *arg[MAXARG];
int arg_num = 0;

int xargs(int argc, char *argv[]) {
    // 0. 将原有的入参先读入 arg 数组中去
    //    若 argc 大于1，执行 argv[1] 所对应的命令
    //    若 argc 为1，执行 echo 命令
    char *cmd;
    if (argc == 1) {
        cmd = "echo";
        arg[arg_num++] = cmd;
    }
    else {
        cmd = argv[1];
        arg[arg_num++] = cmd;
        for (; arg_num < MAXARG && arg_num < argc - 1; arg_num++) arg[arg_num] = argv[arg_num + 1];
    }

    // 1. 从标准输入中读，直到读到 '\n' 换行符
    int pos = 0;        // buf 读到的位置
    int input_bytes;    // 当次 read 读入的字节数

    while ((input_bytes = read(stdin, buf + pos, BUF_MAX - pos - 1)) > 0) {     // 读到 EOF，或者说 ctrl + d
        pos += input_bytes;
        // 若需要：发现输入了换行符就停止继续读，将下面一行注释去掉即可
        // if (strchr(buf, '\n')) break;
    }
    buf[pos] = 0;

    // 2. 将第一步所读到的参数切分，并放入 arg 中
    char *start, *end;
    start = buf, end = buf;
    for (; arg_num < MAXARG; ) {       // 双指针算法
        while (*end != ' ' && *end != '\n' && *end != 0) end++;     // end 跳到第一个空格、换行或EOF
        arg[arg_num++] = start;     // 将 start 和 end 区间作为字符串存入 arg
        if (*end == 0) break;       // 若 end 读到了 EOF，则不用继续扫描了
        *end++ = 0;                 // 用 '\0' 截断字符串
        while (*end == ' ' || *end == '\n') end++;      // 迭代 end，找到第一个非空白的字符
        if (*end == 0) break;       // 若 end 迭代后指向的是 EOF，则不用继续分析了
        start = end;                // 迭代 start
    }

    // 3. 调用fork、exec
    int pid;
    if ((pid = fork()) == 0) {
        if (exec(cmd, arg) < 0) fprintf(2, "xargs: exec fails with %s\n", cmd);     // 子进程
        exit(1);
    }
    else wait(&pid);    // 父进程

    exit(0);
}