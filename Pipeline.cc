#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "Pipeline.h"
#include "CommandLine.h"
#include "TmpFile.h"
#include "VarString.h"
#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
#include "StringProc.h"
using namespace std;

Pipeline::Pipeline(Feeder *f, Environment *env) : Element(f,env)
{
	m_outfile = NULL;
	m_outstr = NULL;

	m_strproc = NULL;

	m_if = false;
}

Pipeline::~Pipeline()
{
	if(m_strproc != NULL)
		delete m_strproc;
}

void Pipeline::print(int indent_level)
{
}

/* parse of a pipe line, which is more than one command lines.
	commandLine ... >>= commandLine ... >>= ...
	or 
	string ... >>= commandLine ... >>= ...
*/
bool Pipeline::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	// when the first part is string.
	if(add(new StringProc(m_feeder,m_env))){
		m_strproc = (StringProc *)m_nodes[0];
		m_nodes.pop_back();
	}else if(add(new CommandLine(m_feeder,m_env))){

	}else{
		return false;
	}

	int comnum = 1;
	while(1){
		while(m_feeder->comment());

		if(! m_feeder->str(">>="))
			break;

		if(add(new CommandLine(m_feeder,m_env))){
			comnum++;
		}else
			break;

	}

	if(comnum < 1){
		m_feeder->setPos(m_start_line,m_start_char);
		return false;
	}


	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

bool Pipeline::eval(void)
{
	((CommandLine *)m_nodes.back())->m_outfile = m_outfile;
	((CommandLine *)m_nodes.back())->m_outstr = m_outstr;
	return true;
}

int Pipeline::exec(void)
{
	if(m_strproc != NULL){
		m_strproc->exec();
		return 0;
	}

	eval();

	// When wait(1) is set in the command line,
	// wait(1) is done in this process.
	// Wait(1) cannot be connected with other commands
	auto *cl = (CommandLine *)m_nodes[0];
	if(cl->m_is_wait){
		return execWait();
	}

	int pip[2];
	int prevfd = -1;

	for(auto *n : m_nodes){
		auto *p = (CommandLine *)n;
		pip[1] = -1;
		if ( p != m_nodes.back() && pipe(pip) < 0) {
			close(prevfd);
			m_error_msg = "Pipe call failed";
			throw this;
		}

		p->setPipe(pip,prevfd);
		m_pids.push_back( p->exec() );
		prevfd = p->getPrevPipe();
	}

	if(m_outstr != NULL){
		m_outstr->readFiFo();
	}

	for(auto pid : m_pids)
		waitCommands(pid);

	return m_exit_status;
}

int Pipeline::execWait(void)
{
	auto *c = (CommandLine *)m_nodes[0];
	c->eval();
	auto argv = c->makeArgv();
	
	int i = 1;
	while(argv[i] != NULL){
		int pid = m_env->getBG(argv[i]);
		while(pid == 0){ // not forked
			pid = m_env->getBG(argv[i]);
		}
		if(pid < 0){
			m_error_msg = "Unknown background process";
			m_exit_status = 1;
			throw this;
		}
		m_pids.push_back(pid);
		m_env->unsetBG(argv[i]);
		i++;
	}

	for(auto pid : m_pids)
		waitCommands(pid);

	if(m_env->m_v_opt)
		cerr << "+ pid " << m_env->m_pid << " wait done" << endl;

	return 0;
}

void Pipeline::waitCommands(int pid)
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
		m_error_msg = "Irregular command termination";
		m_exit_status = 2;
		throw this;
	}

	m_exit_status = WEXITSTATUS(status);
	if(m_env->m_v_opt)
		cerr << "+ pid " << pid << " exit " << m_exit_status << endl;

	if(m_exit_status == 0)
		return;

	if(m_nodes.size() > 1)
		m_error_msg = "Pipeline error";
	else
		m_error_msg = "Command error";

	if(! m_if)
		throw this;
}
