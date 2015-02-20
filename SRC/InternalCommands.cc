#include "InternalCommands.h"
#include "Environment.h"
#include "CommandLine.h"
#include "Feeder.h"
#include <iostream>
#include <string.h>
#include <string>
using namespace std;

bool InternalCommands::exist(string *name)
{
	if(*name == "echo" || *name == "pid")
		return true;

	return false;

}

int InternalCommands::exec(char const** argv,Environment *e,Feeder *f,CommandLine *p)
{
	int c = 0;
	while(argv[c] != NULL){
		c++;
	}
	if(strncmp(argv[0],"echo",4) == 0){
		exit( echo(c,argv) );
	}else if(strncmp(argv[0],"pid",3) == 0){
		exit( pid(c,argv,e) );
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
