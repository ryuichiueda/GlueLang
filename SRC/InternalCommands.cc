// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "InternalCommands.h"
#include "Environment.h"
#include "Exe.h"
#include "Feeder.h"
#include "Script.h"
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
using namespace std;

bool InternalCommands::exist(string *name)
{
	if(*name == "echo" || *name == "pid")
		return true;

	return false;

}

int InternalCommands::exec(char const** argv,Environment *e,Feeder *f,Exe *p)
{
	int c = 0;
	while(argv[c] != NULL){
		c++;
	}
	if(strncmp(argv[0],"echo",4) == 0){
		exit( echo(c,argv) );
	}else if(strncmp(argv[0],"pid",3) == 0){
		exit( pid(c,argv,e) );
		/*
	}else if(strncmp(argv[0],"repeat",6) == 0){
		exit( repeat(c,argv,e) );
	}else if(strncmp(argv[0],"while",5) == 0){
		exit( while_(c,argv,e) );
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

/*
int InternalCommands::repeat(int argc, char const** argv, Environment *e)
{
	char *c = NULL;
	if(argc < 2)
		exit(1);

	long rep = strtol(argv[1], &c, 10);
	if(*c != '\0')
		exit(1);

	string tmpdir = argv[2];
	argv[0] = (char *)tmpdir.c_str();
	int k = 1;
	while(argv[k+2] != NULL){
		argv[k] = argv[k+2];
		k++;
	}
	argv[k] = NULL;

	int es = 0;
	for(int i=0;i<rep;i++){
		int pid = fork();
		if(pid != 0){//parent proc
		        int options = 0;
		        int status;
		        int wpid = waitpid(pid,&status,options);

			if(wpid != pid)
				exit(1);

			if(!WIFEXITED(status)){
				if(WIFSIGNALED(status) && WTERMSIG(status) == 13){
					es = 0;
				}else{
					es = WEXITSTATUS(status);
				}
			}else{
				es = WEXITSTATUS(status);
			}

			if(e->m_v_opt)
				cerr << "+ pid " << pid << " exit " << es << endl;

			if(es != 0)
				exit(es);

			continue;
		}

		//child proc
	        ifstream ifs(argv[0]);
	        Feeder feeder(&ifs);
	
	        e->initExeProc((const char**)argv);
	        Script s(&feeder,e);
	
	        s.setSilent();
	        s.parse();
	        s.exec();
	}

	return 0;
}

int InternalCommands::while_(int argc, char const** argv, Environment *e)
{
	if(argv[1] == NULL)
		exit(0);

	string tmpdir = argv[1];
	argv[0] = (char *)tmpdir.c_str();
	int k = 1;
	while(argv[k+1] != NULL){
		argv[k] = argv[k+1];
		k++;
	}
	argv[k] = NULL;

	int es = 0;
	while(1){
		int pid = fork();
		if(pid != 0){//parent proc
		        int options = 0;
		        int status;
		        int wpid = waitpid(pid,&status,options);

			if(wpid != pid)
				exit(1);

			if(!WIFEXITED(status)){
				if(WIFSIGNALED(status) && WTERMSIG(status) == 13){
					es = 0;
				}else{
					es = WEXITSTATUS(status);
				}
			}else{
				es = WEXITSTATUS(status);
			}

			if(e->m_v_opt)
				cerr << "+ pid " << pid << " exit " << es << endl;

			if(es != 0)
				exit(0);

			continue;
		}

		//child proc
	        ifstream ifs(argv[0]);
	        Feeder feeder(&ifs);
	
	        e->initExeProc((const char**)argv);
	        Script s(&feeder,e);
	
	        s.setSilent();
	        s.parse();
	        s.exec();
	}

	return 0;
}
*/

int InternalCommands::pid(int argc, char const** argv,Environment *e)
{
	cout << e->m_pid << endl;
	return 0;
}
