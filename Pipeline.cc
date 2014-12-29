#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "Pipeline.h"
#include "CommandLine.h"
#include "TmpFile.h"
#include "Arg.h"
#include "Feeder.h"
using namespace std;

Pipeline::Pipeline(Feeder *f, Environment *env) : Element(f,env)
{
	m_outfile = NULL;
}

Pipeline::~Pipeline()
{

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
	int prev_ln,prev_ch;
	m_feeder->getCurPos(&prev_ln, &prev_ch);

	// scanning of file name 
	// If `file <filename> =` is found, 
	// the TmpFile object is pushed as the first element of m_nodes.
	// This node is also given to the last command line.
	if(add(new TmpFile(m_feeder,m_env))){
		m_outfile = (TmpFile *)m_nodes[0];	
	}

	int comnum = 0;
	string tmp;
	while(1){
		bool repeat = false;

		if(add(new CommandLine(m_feeder,m_env))){
			repeat = true;
			comnum++;
		}else{

			errorCheck();
			break;
/*
			if(errorExist()){
				return false; // error exit
			}else{
				break;
			}
*/
		}

		if(! m_feeder->pipe(&tmp))
			break;

		m_feeder->blank(&tmp);

		if(! repeat)
			break;
	}

	if(comnum < 2){
		m_feeder->rewind(prev_ln,prev_ch);
		return false;
	}

	if(m_outfile != NULL){
		((CommandLine *)m_nodes.back())->pushOutFile(m_outfile);
	}
	return true;
}

bool Pipeline::eval(void)
{
	return true;
}

//int Pipeline::fork

int Pipeline::exec(void)
{
	cout << flush;

	vector<int> pids;

	int pip[2];
	int prevfd = -1;
	int n = 0;
	if(m_outfile != NULL)
		n++;

	for(int i=n;i<m_nodes.size();i++){
		auto *p = (CommandLine *)m_nodes[i];
		pip[1] = -1;
		if ( i+1 != (int)m_nodes.size() && pipe(pip) < 0) {
			close(prevfd);
			m_error_messages.push_back("Pipe call failed");
		}

		p->setPipe(pip,prevfd);
		pids.push_back( p->exec() );
		prevfd = p->getPrevPipe();
	}

	for(auto pid : pids){
		int status;
		int options = 0;
		waitpid(pid,&status,options);
		if(WIFEXITED(status)){
			int e = WEXITSTATUS(status);
			if(e != 0)
				return e;
		}
	}
	return 0;
}
