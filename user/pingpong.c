#include <kernel/types.h>
#include <user/user.h>

/*
这个实验，要保证过程的正确性
首先是，先让父进程写，子进程读
这个过程结束之后
才是子进程写，父进程读。如果只创建一对管道的话，那么一定要注重这个过程，否则会发生死锁

如果是两对管道，那么虽然也是上方的过程，但不必担心死锁。
*/
int main(int argc, int *argv[]){
  int pip[2]={0};
  char buf[512];
  pipe(pip);
  int pid=fork();
  if(pid<0){
    fprintf(2,"error fork!\n");
    exit(1);
  }  
  int cpid;
  // 中间会发生死锁吗，因为如果子进程先进行的话，那么就会一直等待向管道中写入，而写入的是父进程做的事情。
  cpid = getpid(); //current pid 
  if(pid==0){
      int len = read(pip[0], buf, 512);
      buf[len]='\0';
      printf("<%d>: received ping\n", cpid);
      write(pip[1], "2", 1);
      //exit(0);
  }else{
      write(pip[1],"1", 1);
      // 还有注意的点，就是向管道写入后，应当是子进程接收到信息，而不是父进程。
      while(1){// 所以，这个while循环就是来解决自发自接情况的
        int len = read(pip[0], buf, 512);
        buf[len]='\0';
        if(buf[0]=='2'){
          printf("<%d>: received pong\n", cpid);
          break;
        }
        write(pip[1], "1", 1);
      }
  }
  return 0;

}