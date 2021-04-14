#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	if(argc < 2){
		fputs("invout <command> [args]\n", stderr);
		exit(1);
	}

	dup2(1, 3);
	dup2(2, 1);
	dup2(3, 2);

	return - execvp(argv[1], &argv[1]);
}
