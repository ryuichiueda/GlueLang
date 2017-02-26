#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "Pipeline.h"
#include "ExeProc.h"
#include "ExeSubShell.h"
#include "ExeString.h"
#include "ExeExtCom.h"
#include "ExeEachline.h"
#include "ExeIntCom.h"
#include "DefFile.h"
#include "DefStr.h"
#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
#include "DataJob.h"
using namespace std;

Pipeline::Pipeline(Feeder *f, Environment *env) : Element(f,env)
{
	m_outfile = NULL;
	m_outstr = NULL;

	m_if = false;

	m_has_else = false;
}

Pipeline::~Pipeline()
{
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

	int comnum = 0;
	while(1){
		bool res = add(new ExeSubShell(m_feeder,m_env))
			|| add(new ExeString(m_feeder,m_env))
 			|| add(new ExeEachline(m_feeder,m_env))
			|| add(new ExeProc(m_feeder,m_env))
			|| add(new ExeIntCom(m_feeder,m_env))
			|| add(new ExeExtCom(m_feeder,m_env));
		if(res)
			comnum++;
		else
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

int Pipeline::exec(void)
{
	m_nodes.back()->m_outfile = m_outfile;
	m_nodes.back()->m_outstr = m_outstr;

	// When wait(1) is set in the command line,
	// wait(1) is done in this process.
	// Wait(1) cannot be connected with other commands
	//auto *cl = (Exe *)m_nodes[0];
	if(((Exe *)m_nodes[0])->m_is_wait)
		return execWait();

	int pip[2];
	int prevfd = -1;

	for(auto *n : m_nodes){
		auto *p = (Exe *)n;
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

	if(m_outstr != NULL)
		m_outstr->readFifo();

	for(auto pid : m_pids)
		waitCommands(pid);

	return m_exit_status;
}

int Pipeline::execWait(void)
{
	auto *c = (Exe *)m_nodes[0];
	c->eval();
	auto argv = c->makeArgv();
	
	int i = 1;
	while(argv[i] != NULL){
		DataJob *p = NULL;
		int pid = 0;
		
		while(pid == 0){ // not forked
			if(p == NULL){
				try{
					string s(argv[i]);
					p = (DataJob *)m_env->getData(&s);
				}catch(...){
					m_error_msg = "Bug of backgound process";
					m_exit_status = 1;
					throw this;
				}
			}
			pid = p->m_pid;
			//pid = m_env->getBG(argv[i]);
		}
		if(pid < 0){
			m_error_msg = "Unknown background process";
			m_exit_status = 1;
			throw this;
		}
		m_pids.push_back(pid);
		p->m_pid = 0;
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

	// judged at the upper level if !> is used
	if(m_has_else)
		return;

	if(m_exit_status == 0)
		return;

	if(m_nodes.size() > 1)
		m_error_msg = "Pipeline error";
	else
		m_error_msg = "Command error";

	if(! m_if)
		throw this;
}
