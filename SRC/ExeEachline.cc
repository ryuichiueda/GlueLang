// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "ExeEachline.h"
//#include "ArgCommand.h"
//#include "InternalCommands.h"
#include "Environment.h"
#include "Script.h"
#include "Arg.h"
#include "Where.h"
#include "DefFile.h"
#include "DefStr.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "ExeProc.h"
#include "ExeString.h"
#include "ExeExtCom.h"
#include "ExeIntCom.h"
using namespace std;

ExeEachline::ExeEachline(Feeder *f, Environment *env) : Exe(f,env)
{
}

ExeEachline::~ExeEachline()
{
}

bool ExeEachline::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);
	if(!m_feeder->str("eachline")){
		return false;
	}

	bool res = add(new ExeProc(m_feeder,m_env))
		|| add(new ExeIntCom(m_feeder,m_env))
		|| add(new ExeExtCom(m_feeder,m_env));

	if(!res){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void ExeEachline::execChild(void)
{	
	auto *com = (Exe *)m_nodes[0];
	string line;
	while (getline(cin, line)){
		//vector<string> ws;
		line += ' ';//add a space at the end of a line
		int from = 0;
		for(int i=from;i<line.size();i++){
			if(line.at(i) == ' '){
				com->appendArg(line.substr(from,i-from));
				from = i + 1;
			}
		}

		int pid = com->exec();
		
		com->clearAppendArg();
		waitCommands(pid);
		if(m_exit_status != 0)
			exit(m_exit_status);
	}

	exit(0);
}

//copy from PipeLine.cc (It's not cool.)
void ExeEachline::waitCommands(int pid)
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
	m_command_error = true;
	throw this;
}
