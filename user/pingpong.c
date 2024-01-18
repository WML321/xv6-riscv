#include <kernel/types.h>
#include <user/user.h>

/*
���ʵ�飬Ҫ��֤���̵���ȷ��
�����ǣ����ø�����д���ӽ��̶�
������̽���֮��
�����ӽ���д�������̶������ֻ����һ�Թܵ��Ļ�����ôһ��Ҫע��������̣�����ᷢ������

��������Թܵ�����ô��ȻҲ���Ϸ��Ĺ��̣������ص���������
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
  // �м�ᷢ����������Ϊ����ӽ����Ƚ��еĻ�����ô�ͻ�һֱ�ȴ���ܵ���д�룬��д����Ǹ������������顣
  cpid = getpid(); //current pid 
  if(pid==0){
      int len = read(pip[0], buf, 512);
      buf[len]='\0';
      printf("<%d>: received ping\n", cpid);
      write(pip[1], "2", 1);
      //exit(0);
  }else{
      write(pip[1],"1", 1);
      // ����ע��ĵ㣬������ܵ�д���Ӧ�����ӽ��̽��յ���Ϣ�������Ǹ����̡�
      while(1){// ���ԣ����whileѭ������������Է��Խ������
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