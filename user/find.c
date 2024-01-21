#include <kernel/types.h>
#include <user/user.h>
#include <kernel/stat.h>
#include <kernel/fs.h>


int find(char *file_name, char *path){
    // opendir打开目录
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    int success=0;
    if ((fd = open(path, 0)) < 0){
        fprintf(2, "cannot open %s\n", path);
    }

    if (fstat(fd, &st) < 0){
        fprintf(2, "cannot stat %s\n", path);
    }

    
    strcpy(buf, path);
    p = buf+strlen(buf);
    //int origin_len = strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0){
            continue;
        }
	if(strcmp(".", de.name)==0){
            continue;
        }
        if(strcmp("..", de.name)==0){
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;//buf新的末尾
	//printf("buf: %s\n", buf);
	//printf("de.name: %s\n", de.name);
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
	//printf("fmtname: %s, strcmp: %d\n", fmtname(buf), strcmp(".", fmtname(buf)));
	
        if(st.type==T_FILE){
	    if(!strcmp(file_name, de.name)){
		printf("%s/%s\n", path, file_name);
		success=1;
	    }
        }else{
            find(file_name, buf);
	    //p = buf+origin_len;
	    //*p++ = '/';
        }
    }
    close(fd);
    return success;
}

int main(int argc, char *argv[]){

    if(argc<2){
        fprintf(2, "parameter numebr error!\n");
        exit(1);
    }
    if(argc==2){
    	find(argv[2], ".");
	return 0;
    }
    char *path = argv[1];
    char *file_name = argv[2];
    find(file_name, path);
    return 0;
}

