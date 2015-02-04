/*
 * Copyright (c) 2015 Mitzyuki IMAIZUMI  All rights reserved.
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<stdarg.h>
#include	<string.h>
#include	<errno.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<sys/resource.h>

/* input buffer size */
#define		BUFFSIZE	1024

/* input line chain */
typedef	struct	list_t {
	char			buff[BUFFSIZE];
	struct	list_t	*next;
}	list_t;

/* prototype */
		char	*readstr(FILE *);
static	char	*mkline(list_t *, size_t);
static	void	*Malloc(size_t),
				Free(void *),
				error(char *, ...);

/*
 * main
 */
int		main(int argc, char *argv[])
{

	char	*p;
	pid_t	pid;

	/* require executable command */
	if(argc != 2)
		error("Usage: %s command\n", argv[0]);

	/* main loop */
	while((p = readstr(stdin))){					/* read from stdin */
		if((pid = fork()) < 0)
			error("fork(%s)\n", strerror(errno));
		else if(pid){								/* parent */
			int		status;

			waitpid(pid, &status, 0);
			if(WEXITSTATUS(status))
				error("%s: error\n", argv[1]);
			Free(p);
		}
		else{										/* child */
			execlp(argv[1], argv[1], p, (char *)NULL);
			error("execlp(%s)\n", strerror(errno));
		}
	}

	exit(0);

}

/*
 * Reads a line from stream, delimited by the newline.  Allocates sufficient
 * memory for store input strings, and returns a pointer to it.  The pointer
 * may subsequently be used as an argument to the function free(3).
 */
char	*readstr(FILE *fp)
{

	list_t	*start = NULL,
			**p = &start;
	size_t	len = 0;

	while(1){
		*p = Malloc(sizeof(**p));
		if(fgets((*p)->buff, BUFFSIZE, fp)){
			size_t	i = strlen((*p)->buff);
			len += i;
			if((*p)->buff[i-1] == '\n')
				return(mkline(start, len));
			p = &((*p)->next);
		}
		else if(feof(fp))
			return(NULL);
		else
			error("fgets(%s)\n", strerror(errno));
	}

}

/*
 * Make NULL terminated string from chain buffer and free it.
 */
static	char	*mkline(list_t *start, size_t len)
{

	list_t	*p = start,
			*q;
	char	*s = Malloc(len + 1);

	while(p){
		strcat(s, p->buff);
		q = p->next;
		Free(p);
		p = q;
	}

	if(s[len-1] == '\n')
		s[len-1] = '\0';

	return(s);

}

/*
 * malloc(3) wrapper.
 */
void	*Malloc(size_t s)
{

	void	*p;

	if(!(p = malloc(s)))
		error("malloc(%s)\n", strerror(errno));

	memset(p, 0, s);

	return(p);

}

/*
 * free(3) wrapper.
 */
void	Free(void *p)
{

	free(p);

}

/*
 * error routine.
 */
void	error(char *fmt, ...)
{

	va_list	ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	exit(1);

}
