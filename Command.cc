#include "Command.h"
#include "Arg.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include "Feeder.h"
using namespace std;

Command::Command(Feeder *f) : Node(f)
{
//	parser(script,pos);
}

Command::~Command()
{

}

void Command::print(int indent_level)
{

}

void Command::setName(string s){
	m_name = s;
	Arg arg(&m_name,m_feeder);
	m_args.push_back(arg);
}

void Command::appendArg(string a){
	Arg arg(&a,m_feeder);
	m_args.push_back(arg);
}

void Command::parse(void)
{
	string com,arg;
	m_feeder->getToken(&com);
	setName(com);
	m_feeder->getToken(&arg);
	appendArg(arg);
}

int Command::exec(void)
{
	cout << flush;

	int pid = fork();
	if(pid < 0)/* error */
		exit(1);

	if (pid == 0){/* child */
		execCommand();
		_exit(127);
	}

	/* parent */
	int status;
	int options = 0;
	waitpid(pid,&status,options);

	return pid;
}

void Command::execCommand(void)
{
	m_feeder->close();

	auto **argv = new const char* [m_args.size()];
	for (int i=0;i < m_args.size();i++)
		argv[i] = m_args[i].getString();

	argv[m_args.size()] = NULL;

	execve(argv[0],(char **)argv,NULL);
}
