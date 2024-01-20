#include<kernel/types.h>
#include<user/user.h>

int sieve(int *numbers, int len){
	int base = numbers[0];
	int new_len=0;
	for(int i=1;i<len;i++){
		int multi = numbers[i]/base;
		if(base*multi!=numbers[i]){
			numbers[new_len++]=numbers[i];
		}
	}
	return new_len;
}


void pipline(int pipe_parent[2]){ // 父进程的管道符作为参数传递进来
	// 这里需要先接收数组和长度
	// 并根据长度进行递归的出口判断
	int len=0;
	//char buf[16];
	close(pipe_parent[1]);
	read(pipe_parent[0],&len,sizeof(int));
	//buf[len]='\0';
	//len = atoi(buf); // 先读个长度
	//printf("len is %d\n", len);
	if(len==0){
		return;
	}
	
	int numbers[len];
	// 再读数组
	for(int i=0;i<len;i++){
		read(pipe_parent[0], &numbers[i], sizeof(int));
		//buf[temp_len]='\0';
		//numbers[i]=atoi(buf);
		//printf("numbers: %d\n", numbers[i]);
	}
	
	// 处理
	printf("prime %d\n", numbers[0]);
	len = sieve(numbers, len);

	// 创建新的管道符
	int pip_child[2];
	pipe(pip_child);
	// 使用fork
	int pid = fork();
	if(pid==0){
	// 新创建的子进程(右)
		pipline(pip_child);
		return ;
	}else{
		// 发送数据
		close(pip_child[0]);
		write(pip_child[1], &len, sizeof(int));
		for(int i=0;i<len;i++){
			write(pip_child[1], &numbers[i], sizeof(int));
		}
		int status=0;
		wait(&status);
		return;
	}

}

int main(int argc, int *argv[]){

	int numbers[34];
	for(int i=2;i<=35;i++){
		numbers[i-2]=i;
	}
	int pip[2];
	pipe(pip);
	//close(pip[0]);

	int pid = fork();
	if(pid==0){
		pipline(pip);
		return 0;
	}else{
		close(pip[0]);
		int l=34;
		write(pip[1], &l, sizeof(int));
		for(int i=0;i<34;i++){
			write(pip[1], &numbers[i], sizeof(int));
			//printf("success write byte: %d \n", tmp);
		}
		int status=0;
		wait(&status);
		return 0;
	}

}
