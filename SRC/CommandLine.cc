#include "CommandLine.h"
#include "ArgCommand.h"
#include "InternalCommands.h"
#include "Environment.h"
#include "Script.h"
#include "Arg.h"
#include "Where.h"
#include "TmpFile.h"
#include "VarString.h"
#include "Literal.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include <string.h>
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
	bool flg = true;
	while(1){
		flg = add(new Literal(m_feeder,m_env))
			|| add(new Arg(m_feeder,m_env));
		if(flg == false)
			return;

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

	eval();

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
		if(m_outfile != NULL)
			m_outfile->exec();
		else if(m_outstr != NULL)
			m_outstr->exec();

		execChild();
		execErrorExit();
	}

	/*parent*/
	parentPipeProc();
	return pid;
}

char** CommandLine::makeArgv(void)
{
	auto argv = new char* [m_nodes.size() + 1 + m_add_args.size()];
	
	argv[0] = (char *)((ArgCommand *)m_nodes[0])->getStr();
	int i = 1;
	for (;i < (int)m_nodes.size();i++){
		argv[i] = (char *)((Arg *)m_nodes[i])->getEvaledString();
	}
	for(auto a : m_add_args){
		argv[i] = new char [a.size()+1];
		strcpy(argv[i],a.c_str());
		argv[i][a.size()] = '\0';
		i++;
	}

	argv[m_nodes.size() + m_add_args.size()] = NULL;
	return argv;
}

bool CommandLine::eval(void)
{
	for(auto s : m_nodes){
		if(s->eval())
			continue;

		m_error_msg = "evaluation of args failed";
		throw this;
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
