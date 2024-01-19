#include <kernel/types.h>
#include <user/user.h>

/*
在这个过程中，要保证顺序的正确性.
因为父子进程是互相通信的，所以父进程不能接受本身发送的消息，子进程也是一样的
如果是用两对管道符，那么上述问题不需考虑，如果是一对管道符，就一定要考虑
下边的代码是一对管道符
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
  cpid = getpid(); //current pid 
  if(pid==0){
      int len = read(pip[0], buf, 512);
      buf[len]='\0';
      printf("<%d>: received ping\n", cpid);
      write(pip[1], "2", 1);
      //exit(0);
  }else{
      write(pip[1],"1", 1);
      // 向管道写入后，接受消息的应该是子进程，而不是父进程
      while(1){// 所以，这里有个while，就是防止接收自身的消息
        int len = read(pip[0], buf, 512);
        buf[len]='\0';
        if(buf[0]=='2'){
          printf("<%d>: received pong\n", cpid); // 说明接收的子进程的消息，可以做后续操作了
          break;
        }
        write(pip[1], "1", 1);// 否则，自己接收自己，需要重新发送。
      }
  }
  return 0;

}

// 但是上方的代码，使用xv6的那个grade评估不通过，但是在shell中是成功运行的
// 下方是使用两对管道符的版本
// 但是如果不加close的话，评估是不通过的，我也不理解，为什么非要加上close不可。程序里，完全不具有产生死锁的条件啊，因为不同的进程中不同时对同意管道进行读或写。
/*
 *int main(int argc, int *atgv[]){
  int pip_1[2]={0}, pip_2[2]={0};
  char buf[512];
  
  pipe(pip_1);
  pipe(pip_2);
  
  int pid=fork();
  if(pid<0){
    fprintf(2,"Error fork!\n");
  }
  
  int cpid = getpid();
  
  // 父进程向子进程发送消息
  if(pid!=0){
    close(pip_1[0]);
    write(pip_1[1], "1", 1);
  }else{
    close(pip_1[1]);
    int len = read(pip_1[0], buf, 512);
    buf[len]='\0';
    printf("%d: received ping\n", cpid);
  }
  
  // 子进程向父进程发送消息
  if(pid==0){
    close(pip_2[0]);
    write(pip_2[1], "2", 1);
  }else{
    close(pip_2[1]);
    int len = read(pip_2[0], buf, 512);
    buf[len]='\0';
    printf("%d: received pong\n", cpid);
  }
  return 0;
}
 * */
