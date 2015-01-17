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
#include "Where.h"
using namespace std;

Pipeline::Pipeline(Feeder *f, Environment *env) : Element(f,env)
{
	m_outfile = NULL;
	m_outstr = NULL;

	m_if = false;

	m_where = NULL;
}

Pipeline::~Pipeline()
{
	if(m_where != NULL)
		delete m_where;
}

void Pipeline::print(int indent_level)
{
}

/* parse of a pipe line, which is more than one command lines.

	file f = command ... >>= command ... >>= ...
	command ... >>= command ... >>= ...

* to do:
	to implement file redirection for standard error, like
	file f1 f2 = command ... 

	The way of writting for redirection of standard error
	toward one command line will be an issue.
*/
bool Pipeline::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	// scanning of file name 
	// If `file <filename> =` is found, 
	// the TmpFile object is pushed as the first element of m_nodes.
	// This node is also given to the last command line.
	if(add(new TmpFile(m_feeder,m_env))){
		m_outfile = (TmpFile *)m_nodes[0];	
		m_nodes.pop_back();
	}else if(add(new VarString(m_feeder,m_env))){
		m_outstr = (VarString *)m_nodes[0];	
		m_nodes.pop_back();
	}

	int comnum = 0;
	while(1){
		if(add(new CommandLine(m_feeder,m_env))){
			if(m_if)
				((CommandLine *)m_nodes.back())->setIfFlag();
			comnum++;
		}else
			break;

		while(m_feeder->comment());

		if(! m_feeder->str(">>=")){
			if(m_feeder->str(">>")){
				m_feeder->setPos(m_start_line, m_start_char);
				return false;
			}
			
			break;
		}
	}

	if(comnum < 1){
		m_feeder->setPos(m_start_line,m_start_char);
		return false;
	}

	((CommandLine *)m_nodes.back())->m_outfile = m_outfile;
	((CommandLine *)m_nodes.back())->m_outstr = m_outstr;

	if(add(new Where(m_feeder,m_env))){
		m_where = (Where *)m_nodes.back();
		m_nodes.pop_back();
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

bool Pipeline::eval(void)
{
	return true;
}

int Pipeline::exec(void)
{
	if(m_where != NULL)
		m_where->exec();

	cout << flush;

	vector<int> pids;

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
		pids.push_back( p->exec() );
		prevfd = p->getPrevPipe();
	}

	if(m_outstr != NULL){
		m_outstr->readFiFo();
	}
	for(auto pid : pids){
		int status;
		int options = 0;
		waitpid(pid,&status,options);
		if(WIFEXITED(status)){
			int e = WEXITSTATUS(status);
			if(e == 0)
				continue;

			if(m_nodes.size() > 1)
				m_error_msg = "Pipeline error";
			else
				m_error_msg = "Command error";
			m_exit_status = e;
			if(! m_if)
				throw this;
		}
	}
	return m_exit_status;
}
