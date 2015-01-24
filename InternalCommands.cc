#include "InternalCommands.h"
#include "Environment.h"
#include "CommandLine.h"
#include "Feeder.h"
#include <iostream>
#include <string.h>
using namespace std;

int InternalCommands::exec(int argc, char const** argv,Environment *e,Feeder *f,CommandLine *p)
{
	if(strncmp(argv[0],"echo",4) == 0){
		exit( echo(argc,argv) );
	}else if(strncmp(argv[0],"pid",3) == 0){
		exit( pid(argc,argv,e) );
/*
	}else if(strncmp(argv[0],"wait",4) == 0){
		exit( gluewait(argc,argv,e,p) );
*/
	}
	return -1;
}

int InternalCommands::echo(int argc, char const** argv)
{
	for(int i = 1; i < argc-1; i++){
		cout << argv[i] << " ";
	}
	if(argc > 1)
		cout << argv[argc-1] << endl;

	return 0;
}

int InternalCommands::pid(int argc, char const** argv,Environment *e)
{
	cout << e->m_pid << endl;
	return 0;
}

/*
int InternalCommands::subwait(int pid,Environment *e,CommandLine *p)
{
	int options = 0;
	int status;
	int wpid = waitpid(pid,&status,options);
	if(wpid < 1){
		p->m_error_msg = "Command wait error";
		p->m_exit_status = 2;
		throw p;
	}
		
	if(!WIFEXITED(status)){
		p->m_error_msg = "Irregular command termination";
		p->m_exit_status = 2;
		throw p;
	}

	int es = WEXITSTATUS(status);
	if(e->m_v_opt)
		cerr << "+ pid " << pid << " exit " << es << endl;

	if(es == 0)
		return 0;

	throw p;
}

int InternalCommands::gluewait(int argc, char const** argv,Environment *e,CommandLine *p)
{
	for(int i=1;i<argc;i++){
		int pid = e->getBG(argv[i]);
		while(pid == 0){ // not forked
		}
		int es = subwait(pid,e,p);
		if(es != 0)
			exit(es);
	}
	exit(0);
}
*/
