#include "CommandLine.h"
#include "ArgCommand.h"
#include "InternalCommands.h"
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
#include "GlueString.h"
using namespace std;

CommandLine::CommandLine(Feeder *f, Environment *env) : Element(f,env)
{
	m_pipe[0] = -1;
	m_pipe[1] = -1;
	m_pipe_prev = -1;

	m_is_wait = false;

	m_if = false;
}

CommandLine::~CommandLine()
{
}

void CommandLine::parseArgs(void)
{
	while(add(new Arg(m_feeder,m_env))){
		if(m_feeder->comment() || m_feeder->atNewLine())
			return;

		m_feeder->blank();
	}
}

void CommandLine::parentPipeProc(void)
{
	if(m_pipe_prev >= 0)
		close(m_pipe_prev);

	m_pipe_prev = m_pipe[0];
	close(m_pipe[1]);
}

void CommandLine::childPipeProc(void)
{
	if(m_pipe[1] >= 0)
		close(m_pipe[0]);

	if(m_pipe_prev > 0) {
		dup2(m_pipe_prev, 0);
		close(m_pipe_prev);
	}
	if(m_pipe[1] > 1){
		dup2(m_pipe[1], 1);
		close(m_pipe[1]);
	}
}

void CommandLine::execErrorExit(void)
{
	m_error_msg =  "Command error";
	m_exit_status = 127;
	throw this;
}

int CommandLine::exec(void)
{
	cout << flush;

	if(! eval())
		return -1;

	int pid = fork();
	if(pid < 0)
		exit(1);

	if (pid == 0){//child
		if(m_env->m_v_opt)
			cerr << "+ pid " << getpid() << " fork " << endl;

		m_env->m_level++;
		childPipeProc();

		//The child process should not access to the source code.
		m_feeder->close();

		//open a file or a string variable
		if(m_outfile != NULL){
			if(m_outfile->exec() != 0){
				m_error_msg = "Cannot prepare file";
				m_exit_status = 1;
				throw this;
			}
		}else if(m_outstr != NULL){
			if(m_outstr->exec() != 0){
				m_error_msg = "Cannot prepare file";
				m_exit_status = 1;
				throw this;
			}
		}

		execChild();
		execErrorExit();
	}

	/*parent*/
	parentPipeProc();
	return pid;
}

const char** CommandLine::makeArgv(void)
{
	auto argv = new const char* [m_nodes.size() + 2];
	
	auto *com = (ArgCommand *)m_nodes[0];
	argv[0] = com->getStr();

	int skip = 0;

	for (int i=1;i < (int)m_nodes.size();i++){
		argv[i+skip] = ((Arg *)m_nodes[i])->getEvaledString();
	}

	argv[m_nodes.size()+skip] = NULL;
	return argv;
}

bool CommandLine::eval(void)
{
	for(auto s : m_nodes){
		if( ! s->eval() ){
			m_error_msg = "evaluation of args failed";
			throw this;
		}
	}
	return true;
}

void CommandLine::setPipe(int *pip,int prev)
{
	m_pipe[0] = pip[0];
	m_pipe[1] = pip[1];
	m_pipe_prev = prev;
}

void CommandLine::vOptProc(char const* arg)
{
	if(!m_env->m_v_opt)
		return;

	cerr << "+ pid " << getpid() << " exec line " 
		<< m_start_line << " " << arg << endl;
}
