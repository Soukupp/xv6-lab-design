// find.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* file_name){
    char buf[256];
    int fd;
    struct stat st;
    struct dirent de;

    fd = open(path,0);
    if(fd<0)
    {
        printf("error: find() cannot open %s\n",path);
        exit(1);
    }
    if(fstat(fd,&st)<0){      //获取文件的状态
        printf("error: fstat() cannot stat %s\n",path);
        close(fd);
        exit(1);
    }

    if(st.type == T_FILE){
        if(strcmp(path+strlen(path)-strlen(file_name),file_name)==0){
            printf("%s\n",path);
        }
    }
    else if(st.type == T_DIR){
        if(strlen(path)+1+DIRSIZ+1 > sizeof(buf)){
            printf("error: too long path\n");
            exit(1);
        }
        strcpy(buf, path);
        char* p = buf+strlen(buf);
        // 路径末尾加上'/'
        *p++ = '/';
        while(read(fd,&de,sizeof(de))== sizeof(de)){
            if(de.inum == 0)
                continue;
            memmove(p,de.name,DIRSIZ);
            p[DIRSIZ]=0;
            if(stat(buf,&st)<0){
                printf("error: find() cannot stat %s\n",buf);
                continue;
            }
            // 不进入'/.'和'/..'
            if(strcmp(buf+strlen(buf)-2,"/.")!=0 && strcmp(buf+strlen(buf)-3,"/..")!=0){
                find(buf,file_name);
            }
        }
    }
    close(fd);
}

int main(int argc, char* argv[]){
    if(argc<3)
        exit(1);
    char file_name[256];
    file_name[0]='/';
    strcpy(file_name+1,argv[2]);
    find(argv[1], file_name);
    exit(0);
}
