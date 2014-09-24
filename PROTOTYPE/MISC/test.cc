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
/*
		int flags = fcntl(fd[1], F_GETFD);
		fcntl(fd[1], F_SETFD, flags | FD_CLOEXEC);
		int flags = fcntl(STDOUT_FILENO, F_GETFD);
		fcntl(STDOUT_FILENO, F_SETFD, flags | FD_CLOEXEC);
*/

		char *arg[] = {(char *)"/bin/cat",(char *)"/etc/passwd",NULL};
		execv(arg[0],arg);
		exit(127); 
	}

//	fcntl(fd[0], F_SETFL, O_NONBLOCK);
	const int bsize = 257;
	char buf[bsize];

	int n = 0;
	while(1){
		n = read(fd[0],buf,bsize-1);
		if(n != bsize-1){
			break;
		}
		buf[n] = '\0';
/*
		if(errno == EAGAIN)
			break;
*/
		write(1,buf,n);
		//puts(buf);
		//printf("parents: %d,%d: %s\n",n,errno, buf);
	}
	if(n >= 0){
		buf[n] = '\0';
		write(1,buf,n);
	}
	//printf("parents: %d,%d: %s\n",n,errno, buf);

	int status;
	waitpid(pid,&status,WUNTRACED);
	close(fd[0]);
	exit(0);
}