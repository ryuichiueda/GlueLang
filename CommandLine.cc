#include "CommandLine.h"
#include "Command.h"
#include "InternalCommands.h"
#include "Environment.h"
#include "Arg.h"
#include "StringArray.h"
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

	m_is_strout = false;

	m_if = false;
	m_is_wait = false;
}

CommandLine::~CommandLine()
{
}

/* parse of command line, where command line means
 * the combination of one command, args, and where.

	m_nodes: command arg arg ...
	or
	m_nodes: string procedure
*/
bool CommandLine::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	// start from a literal or an array of literals
	if(add(new StringArray(m_feeder,m_env))){
		m_feeder->getPos(&m_end_line, &m_end_char);
		m_is_strout = true;
		return true;
	}

	// start from a command
	if(!add(new Command(m_feeder,m_env)))
		return false;

	if(!m_feeder->comment() && !m_feeder->atNewLine()){
		m_feeder->blank();
		parseArgs();
	}

	auto *c = (Command *)m_nodes[0];
	if(c->m_is_internal && c->m_name == "wait"){
		m_is_wait = true;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
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
		execCommandLine();
		execErrorExit();
	}

	/*parent*/
	parentPipeProc();
	return pid;
}

const char** CommandLine::makeArgv(void)
{
	if(m_is_strout){
		return ((StringArray *)m_nodes[0])->makeArgv();
	}
	auto argv = new const char* [m_nodes.size() + 2];
	
	Command *com = (Command *)m_nodes[0];
	argv[0] = com->getStr();

	int skip = 0;
	// in the case of proc, argv[0] is glue and argv[1] is
	// the procedure file.
	if(com->m_is_proc){
		skip = 1;
		argv[1] = argv[0];
		argv[0] = m_env->m_glue_path.c_str();
	}

	for (int i=1;i < (int)m_nodes.size();i++){
		argv[i+skip] = ((Arg *)m_nodes[i])->getEvaledString();
	}

	argv[m_nodes.size()+skip] = NULL;
	return argv;
}

void CommandLine::execCommandLine(void)
{
	//The child process should not access to the source code.
	m_feeder->close();

	//open a file or a string variable
	if(m_outfile != NULL){
		if(m_outfile->exec() != 0)
			return;

	}else if(m_outstr != NULL){
		if(m_outstr->exec() != 0)
			return;
	}

	// send the shell level for the case where the child is glue.
	string lv = "GLUELEVEL=" + to_string(m_env->m_level);
	if(putenv((char *)lv.c_str()) != 0){
		m_error_msg = "putenv error";
		m_exit_status = 1;
		throw this;
	}

	auto argv = makeArgv();
	if(m_env->m_v_opt)
		cerr << "+ pid " << getpid() << " exec line " 
			<< m_start_line << " " << argv[0] << endl;

	if(m_is_strout){ // execution of string output
		InternalCommands::exec(argv,m_env,m_feeder,this);
	}else if(((Command *)m_nodes[0])->m_is_internal){// execution of internal command
		InternalCommands::exec(argv,m_env,m_feeder,this);
	}else{
		execv(argv[0],(char **)argv);
	}
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
