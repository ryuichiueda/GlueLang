#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "AndLine.h"
#include "CommandLine.h"
#include "TmpFile.h"
#include "VarString.h"
#include "Arg.h"
#include "Feeder.h"
using namespace std;

Andline::Andline(Feeder *f, Environment *env) : Element(f,env)
{
	m_outfile = NULL;
	m_outstr = NULL;

	m_if = false;
}

Andline::~Andline()
{

}

void Andline::print(int indent_level)
{
}

/* parse of a "and line", which is more than one command lines.

	file f = command ... >>= command ... >> ...
	command ... >> command ... >>= ...

* to do:
	to implement file redirection for standard error, like
	file f1 f2 = command ... 

	The way of writting for redirection of standard error
	toward one command line will be an issue.
*/
bool Andline::parse(void)
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

		if(! m_feeder->str(">>"))
			break;
	}

	if(comnum < 1){
		m_feeder->setPos(m_start_line,m_start_char);
		return false;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);

	// stdout of all commands are appended into a file
	for(auto *n : m_nodes){
		((CommandLine *)n)->m_outfile = m_outfile;
		((CommandLine *)n)->m_outstr = m_outstr;
	}

	return true;
}

bool Andline::eval(void)
{
	return true;
}

int Andline::exec(void)
{
	cout << flush;

	for(int i=0;i<(int)m_nodes.size();i++){
		auto *p = (CommandLine *)m_nodes[i];
		if(m_outfile != NULL){
			if(i!=0){
				m_outfile->m_append_mode = true;
			}
		}
		p->exec();

		if(m_outstr != NULL){
			m_outstr->readFiFo();
		}
		int pid = 0;
		int status;
		int options = 0;
		waitpid(pid,&status,options);
		if(WIFEXITED(status)){
			int e = WEXITSTATUS(status);
			if(e == 0)
				continue;

			m_error_msg = "Andline error";
			m_exit_status = e;
			if(! m_if)
				throw this;
		}
	}

/*
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

			m_error_msg = "Andline error";
			m_exit_status = e;
			if(! m_if)
				throw this;
		}
	}
	return m_exit_status;
*/
	return 0;
}
