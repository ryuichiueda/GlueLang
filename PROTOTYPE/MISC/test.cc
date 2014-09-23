#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
using namespace std;

int main(int argc, char const* argv[])
{
	int fd[2]; 
	const int bsize = 256;
	pipe(fd);  

	sigignore(SIGCHLD);
	int pid = fork();

	if( pid == 0 ){ 
		close(STDOUT_FILENO); 
		dup2(fd[1],STDOUT_FILENO); 
		close(fd[0]);

		printf("this is test!\n");
		char *arg[] = {"-F",NULL};
		char *env[] = {NULL};
		execve("/bin/ls",arg,env);
		exit(0); 
	}

	char buf[bsize];
	read(fd[0],buf,bsize);
	printf("*%s\n",buf);
	return 0;
}
