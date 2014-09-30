#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
using namespace std;

bool s_chld = false;

static void sig_hup(int sig)
{
	fprintf(stderr,"sig_hup");
}

static void sig_chld(int sig)
{
	fprintf(stderr,"sig_chld");
	s_chld = true;
}

static void sig_pipe(int sig)
{
	fprintf(stderr,"sig_pipe");
}

int main(int argc, char const* argv[])
{
	int fd[2]; 
	pipe(fd);  

	struct sigaction sig;
	memset(&sig,'\0',sizeof(sig));

	sig.sa_handler = sig_hup;
	sigaction(SIGHUP,&sig,NULL);
	sig.sa_handler = sig_chld;
	sigaction(SIGCHLD,&sig,NULL);
	sig.sa_handler = sig_pipe;
	sigaction(SIGPIPE,&sig,NULL);

	//sigignore(SIGCHLD);
	int pid = fork();

	if( pid == 0 ){ 
		close(1); 
		dup2(fd[1],1); 
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

	fcntl(fd[0], F_SETFL, O_NONBLOCK);
	const int bsize = 257;
	char buf[bsize];

	int n = 0;
	while(!s_chld){
		n = read(fd[0],buf,bsize-1);
		write(1,buf,n);
	}
	if(n >= 0){
		buf[n] = '\0';
		write(1,buf,n);
	}
	//printf("parents: %d,%d: %s\n",n,errno, buf);

	int status;
	wait(&status);
	close(fd[0]);
	exit(0);
}
