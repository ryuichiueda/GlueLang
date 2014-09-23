#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
using namespace std;

int main(int argc, char const* argv[])
{
	int fd[2]; 
	pipe(fd);  

	//sigignore(SIGCHLD);
	int pid = fork();

	if( pid == 0 ){ 
		close(STDOUT_FILENO); 
		dup2(fd[1],STDOUT_FILENO); 
		close(fd[0]);
/*
		int flags = fcntl(fd[1], F_GETFD);
		fcntl(fd[1], F_SETFD, flags | FD_CLOEXEC);
*/

		char *arg[] = {(char *)"/bin/cat",(char *)"/etc/passwd",NULL};
		execv(arg[0],arg);
		exit(127); 
	}

	fcntl(fd[0], F_SETFL, O_NONBLOCK);
	const int bsize = 256;
	char buf[bsize];
/*
	int status;
	waitpid(pid,&status,WUNTRACED);
*/
	while(1){
		int n = read(fd[0],buf,bsize);
		if(n == EOF){
			buf[0] = '\0';
		//	printf("%d\n",errno);
			continue;
		}
/*
		if(errno == EAGAIN)
			break;
*/
		printf("parents: %d,%d: %s\n",n,errno, buf);
		buf[0] = '\0';
	}
	close(fd[0]);
	return 0;
}
