// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "Executable.h"
#include "Environment.h"
#include "Script.h"
#include "ArrayVariable.h"
#include "ArgVariable.h"
#include "ArgProc.h"
#include "ArgIntCom.h"
#include "Where.h"
#include "DefFile.h"
#include "DefStr.h"
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

Executable::Executable(Feeder *f, Environment *env, vector<int> *scopes) : Element(f,env,scopes)
{
	m_pipe[0] = -1;
	m_pipe[1] = -1;
	m_pipe_prev = -1;
	m_is_wait = false;
}

Executable::~Executable()
{
}

void Executable::parseArgs(void)
{
	bool flg = true;
	while(1){
		flg = add(new Literal(m_feeder,m_env,&m_scopes))
			|| add(new ArrayVariable(m_feeder,m_env,&m_scopes))
			|| add(new ArgVariable(m_feeder,m_env,&m_scopes));

		if(flg == false)
			return;

		if(m_feeder->comment() || m_feeder->atNewLine())
			return;

		m_feeder->blank();
	}
}

void Executable::parentPipeProc(void)
{
	if(m_pipe_prev >= 0)
		close(m_pipe_prev);

	m_pipe_prev = m_pipe[0];
	close(m_pipe[1]);
}

void Executable::childPipeProc(void)
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

void Executable::execErrorExit(void)
{
	m_error_msg =  "Command error";
	m_exit_status = 1;
	throw this;
}

int Executable::exec(DefFile *f, DefStr *s)
{
	cout << flush;

	eval();

	int pid = fork();
	if(pid < 0)
		exit(4);

	if (pid == 0){//child
		if(m_env->m_v_opt)
			cerr << "+ pid " << getpid() << " fork " << endl;

		m_env->m_level++;
		childPipeProc();

		//The child process should not access to the source code.
		m_feeder->close();

		//open a file or a string variable
		if(f != NULL)
			f->connect();
		else if(s != NULL)
			s->connect();

		execChild(f,s);
		execErrorExit();
	}

	/*parent*/
	parentPipeProc();
	return pid;
}

char** Executable::makeArgv(void)
{
	auto argv = new char* [m_nodes.size() + 1 + m_add_args.size()];
	
	argv[0] = (char *)((ArgIntCom *)m_nodes[0])->m_evaled_text.c_str();

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

bool Executable::eval(void)
{
	for(auto &s : m_nodes){
		if(s->eval())
			continue;

		m_error_msg = "evaluation of args failed";
		throw this;
	}
	return true;
}

void Executable::setPipe(int *pip,int prev)
{
	m_pipe[0] = pip[0];
	m_pipe[1] = pip[1];
	m_pipe_prev = prev;
}

void Executable::vOptProc(char const* arg)
{
	if(!m_env->m_v_opt)
		return;

	cerr << "+ pid " << getpid() << " exec line " 
		<< m_start_line << " " << arg << endl;
}
