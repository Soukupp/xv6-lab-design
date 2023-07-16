// pingpong.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD_NUM 1
#define WT_NUM 1
#define RD 0
#define WT 1

int main(int argc, char **argv) {
	int pip_c[2], pip_p[2];
	char buf;
	pipe(pip_c); 
	pipe(pip_p); 
	
	if(fork() != 0) {  // parent process
		write(pip_c[WT], "A", WT_NUM); 
		read(pip_p[RD], &buf, RD_NUM); 
		printf("%d: received pong\n", getpid()); 
		wait(0);
	} else {           // child process
		read(pip_c[RD], &buf, RD_NUM); 
		printf("%d: received ping\n", getpid());
		write(pip_p[WT], &buf, WT_NUM); 
	}
	exit(0);
}


