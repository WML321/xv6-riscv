// 从shell中获取命令和参数，且获取的内容均为string类型
// 最终使用系统调用sleep来实现
// 如果有错误，则输出。
#include <kernel/types.h> //  定义了常用的类型
#include <user/user.h> // 定义了一些用户函数，例如read

int main(int argc, char *argv[]){
	if(argc<=1){
		fprintf(2, "missing parameters!");
		exit(1);
	}
	char *misc = argv[1];
	int sec = atoi(misc);
	int result = sleep(sec);
	printf("exit code %d\n", result);
	exit(result);
}

