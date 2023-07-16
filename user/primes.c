// primes.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stdint.h>

#define RD 0
#define WT 1

void primes_sieve(int pip[2]){
    int32_t res;
    read(pip[RD],&res,sizeof(res));

    if(res==-1)
        exit(0);
    else{
        printf("prime %d\n",res);
        int pip2[2];
        pipe(pip2);
        if(fork()==0){
            close(pip[RD]);
            close(pip2[WT]);
            primes_sieve(pip2);
        }
        else{
            int32_t input;
            close(pip2[RD]);
            while(read(pip[RD],&input,sizeof(input))&&input!=-1){
                if(input%res!=0)
                    write(pip2[WT],&input,sizeof(input));
            }
            input = -1;
            write(pip2[WT],&input,sizeof(input));
            wait(0);
            exit(0);
        }
        
    }
}

int main(int argc, char* argv[]){
    int pip[2];
    pipe(pip);

    if(fork()==0){
        close(pip[WT]);
        primes_sieve(pip);
        exit(0);
    }
    else{
        close(pip[RD]);
        int32_t input;
        for(input=2;input<=35;input++){
            write(pip[WT],&input,sizeof(input));
        }
        input = -1;
        write(pip[WT],&input,sizeof(input));
    }
    wait(0);
    exit(0);

}