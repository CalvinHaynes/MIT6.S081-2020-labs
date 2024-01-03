#include "kernel/types.h"
#include "user/user.h"
#define PRIME_MAX 35

int primefork(int *p){

  int new_p[2];
  //标准输入是没用的，利用一下
  close(0);
  dup(p[0]);
  close(p[0]);
  close(p[1]);

  char buf[4];
  
  if(read(0,buf,4)){
    int num = (int)buf[0];
    //本来pipe放在外面，这样会导致开启无效文件描述符
    pipe(new_p);
    printf("prime %d\n",num);
    while(read(0,buf,4)){
      int new_num = (int)buf[0];
      if(new_num%num != 0){
        write(new_p[1],buf,4);
      }
    }
    if(fork()==0){
      primefork(new_p);
      exit(0);
    }
    else{
      close(new_p[1]);
    }
  }
  else{
    close(0);
  }
  wait(0);  
  return 0;
}

int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "usage: primes\n");
    exit(1);
  }
  int p[2];
  pipe(p);
  for(int i=2;i<=PRIME_MAX;i++){
    write(p[1],&i,4);
  }
  if(fork()==0){
    primefork(p);
    exit(0);
  }
  else{
    close(p[0]);
    close(p[1]);
  }
  wait(0);

  return 0;
}
