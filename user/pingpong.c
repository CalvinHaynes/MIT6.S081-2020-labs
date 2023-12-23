#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "usage: pingpong\n");
    exit(1);
  }
  int p[2];
  pipe(p);
  char buf[2];
  if(fork() == 0) {
    if(read(p[0],buf,1) >0){
      printf("%d: received ping\n",getpid());
    }
    else{
      printf("received ping error\n",getpid());
    }
    write(p[1], buf, 1);
    close(p[1]);
    close(p[0]);
  } else {
    write(p[1], "a", 1);
    sleep(1);
    if(read(p[0],buf,1) >0){
      printf("%d: received pong\n",getpid());
    }
    else{
      printf("received pong error\n",getpid());
    }
    close(p[1]);
    close(p[0]);
}
  exit(0);
}
