#include "EachLine.h"
//#include "ArgCommand.h"
//#include "InternalCommands.h"
#include "Environment.h"
#include "Script.h"
#include "Arg.h"
#include "Where.h"
#include "TmpFile.h"
#include "VarString.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "SubShell.h"
#include "StringPut.h"
#include "ExtCommand.h"
#include "IntCommand.h"
using namespace std;

EachLine::EachLine(Feeder *f, Environment *env) : CommandLine(f,env)
{
}

EachLine::~EachLine()
{
}

bool EachLine::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);
	if(!m_feeder->str("eachline")){
		return false;
	}

	bool res = add(new SubShell(m_feeder,m_env))
		|| add(new IntCommand(m_feeder,m_env))
		|| add(new ExtCommand(m_feeder,m_env));

	if(!res){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void EachLine::execChild(void)
{	
	auto *com = (CommandLine *)m_nodes[0];
	string line;
	while (getline(cin, line)){
		vector<string> ws;
		int s = 0;
		int num = 0;
		line += ' ';
		for(char c : line){
			if(c == ' '){
				com->appendArg(line.substr(s,num));
				num = 0;
				s += num + 2;
			}else
				num++;
		}
		//ws.push_back(line.substr(s,e-s));

/*
		for(auto w : ws){
			//com->appendArg(&w);
			//cerr << &w << endl;
		}
*/

		int pid = com->exec();
		
		com->clearAppendArg();
		waitCommands(pid);
		if(m_exit_status != 0)
			exit(m_exit_status);
	}

	exit(0);
}

//copy from PipeLine.cc (It's not cool.)
void EachLine::waitCommands(int pid)
{
	int options = 0;
	int status;
	int wpid = waitpid(pid,&status,options);
	if(wpid < 1){
		m_error_msg = "Command wait error";
		m_exit_status = 2;
		throw this;
	}
		
	if(!WIFEXITED(status)){
		if(WIFSIGNALED(status) && WTERMSIG(status) == 13){//ignore sigpipe
			// The exit status of the command that is suffered a sigpipe
			// should be zero because the command itself does not anything bad. 
			m_exit_status = 0;
		}else{
			m_error_msg = "Irregular command termination";
			m_exit_status = 2;
			throw this;
		}
	}else{
		m_exit_status = WEXITSTATUS(status);
	}

	if(m_env->m_v_opt)
		cerr << "+ pid " << pid << " exit " << m_exit_status << endl;

	if(m_exit_status == 0)
		return;

	m_error_msg = "Command error";

	if(! m_if)
		throw this;
}
