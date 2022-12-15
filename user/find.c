#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void
find(char *path,char *target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0 || strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                // 添加当前的目录/文件名
                int loc = strlen(path);
                path[strlen(path)] = '/';
                strcpy(path + strlen(path),de.name);

                if (st.type == 2 && strcmp(de.name,target) == 0) //是文件且名字符合
                {
                    printf("%s \n",path);
                } else if(st.type == 1)
                {
                    find(path,target);
                }
                int len = strlen(path);
                for (int i = loc; i < len; ++i) {
                    path[i] = 0;
                }
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc < 2){
        exit(0);
    }
    char pre[100];
    strcpy(pre,argv[1]);
    find(pre,argv[2]);
    exit(0);
}