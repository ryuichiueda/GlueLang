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
using namespace std;

Pipeline::Pipeline(Feeder *f, Environment *env) : Element(f,env)
{
	m_outfile = NULL;
	m_outstr = NULL;

	m_if = false;
}

Pipeline::~Pipeline()
{
}

void Pipeline::print(int indent_level)
{
}

/* parse of a pipe line, which is more than one command lines.
	commandLine ... >>= commandLine ... >>= ...
*/
bool Pipeline::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	int comnum = 0;
	while(1){
		if(add(new CommandLine(m_feeder,m_env))){
			comnum++;
		}else
			break;

		while(m_feeder->comment());

		if(! m_feeder->str(">>="))
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
	eval();

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
