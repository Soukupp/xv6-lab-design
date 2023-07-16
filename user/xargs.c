// xargs.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void my_exec(char* argv, char** ar_set){
    if(fork()==0){
        exec(argv,ar_set);
        exit(0);
    }
    return;
}

int main(int argc, char* argv[]){
    char buf[1024];
    char* ar_set[128];
    char** p = ar_set;

    // 存储用户输入的每个参数的地址
    for(int i=1;i<argc;i++){
        *p= argv[i];
        p++;
    }

    char *p_buf = buf;
    char *p_head = buf;

    // 读入参数
    while(read(0,p_buf,1)!=0){
        if(*p_buf == ' '||*p_buf == '\n'){
            *p_buf = '\0';
            *p = p_head;
            p++;
            p_head = p_buf+1;

            if(*p_buf == '\n'){
                *p = '\0';     
                // 运行ar_set中的各个指令
                my_exec(argv[1],ar_set);
                p = ar_set;
            }
        }
        p_buf++;
    }
    // 对于最后一行进行处理（最后一行可能没有运行run）
    if(ar_set!=p){
        *p_buf ='\0';
        *p = p_head;
        p++;
        *p ='\0';
        my_exec(argv[1], ar_set);
    }
    wait(0);
    exit(0);
}
