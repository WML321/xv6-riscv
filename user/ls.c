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
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){// 看下边的代码，只能说st既适用于目录，又适用于文件
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE: // 指定的path，指向了一个文件
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR: // 指定的path，指向了一个目录
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n"); // 这个长度判断我是不理解的
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/'; // 注意，此时p指向的是buf的末尾
    while(read(fd, &de, sizeof(de)) == sizeof(de)){ // 打开kerneal/看到这样一句话：Directory is a file containing a sequence of dirent structures
	    // 这说明，使用open打开目录，其由众多dirent结构组成的，所以使用文件描述符读取dirent结构体。
      if(de.inum == 0) // 目前还不知道代表什么意思
        continue;
      memmove(p, de.name, DIRSIZ); // 将读取到的name拼接到buf
      p[DIRSIZ] = 0;//buf新的末尾
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);// 读取目录or文件的信息
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
