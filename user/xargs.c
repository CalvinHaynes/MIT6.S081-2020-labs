#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//大多数命令都不接受标准输入作为参数，只能直接在命令行输入参数，这导致无法用管道命令传递参数。
//xargs命令的作用，是将标准输入转为命令行参数
int main(int argc, char *argv[])
{
    //先读取xargs后的
    char* args[32];
    int j = 0;
    for (int i = 1; i < argc; ++i) {
        args[j++] = argv[i];
    }

    //再读取xargs之前的
    char buf[64];
    read(0,buf,64);

    int len = strlen(buf);
    int startLoc = 0;
    for (int i = 0; i < len; ++i) {
        if (buf[i] == 32 || buf[i] == '\n')
        {
            buf[i] = 0;
            args[j++] = buf + startLoc;
            startLoc = i + 1;
        }
    }
    exec(args[0],args);
    exit(0);
}