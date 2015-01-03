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
using namespace std;

Pipeline::Pipeline(Feeder *f, Environment *env) : Element(f,env)
{
	m_outfile = NULL;
	m_outstr = NULL;
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
	m_feeder->getPos(&prev_ln, &prev_ch);

	// scanning of file name 
	// If `file <filename> =` is found, 
	// the TmpFile object is pushed as the first element of m_nodes.
	// This node is also given to the last command line.
	if(add(new TmpFile(m_feeder,m_env))){
		m_outfile = (TmpFile *)m_nodes[0];	
	}else if(add(new VarString(m_feeder,m_env))){
		m_outstr = (VarString *)m_nodes[0];	
	}

	int comnum = 0;
	while(1){
		bool repeat = false;

		if(add(new CommandLine(m_feeder,m_env))){
			repeat = true;
			comnum++;
		}/*else{
			int ln,ch;
			m_feeder->getPos(&ln, &ch);
			m_error_msg = "parse error at line: "
				 + to_string(ln) + ", char: " + to_string(ch);
			throw this;
			errorCheck();
		}*/

		if(! m_feeder->pipe(NULL))
			break;

		m_feeder->blank(NULL);

		if(! repeat)
			break;
	}

	if(comnum < 2){
		m_feeder->setPos(prev_ln,prev_ch);
		return false;
	}

	if(m_outfile != NULL){
		((CommandLine *)m_nodes.back())->pushOutFile(m_outfile);
	}else if(m_outstr != NULL){
		((CommandLine *)m_nodes.back())->pushVarString(m_outstr);
	}
	return true;
}

bool Pipeline::eval(void)
{
	return true;
}

int Pipeline::exec(void)
{
	cout << flush;

	vector<int> pids;

	int pip[2];
	int prevfd = -1;
	int n = 0;
	if(m_outfile != NULL || m_outstr != NULL)
		n++;

	for(int i=n;i<(int)m_nodes.size();i++){
		auto *p = (CommandLine *)m_nodes[i];
		pip[1] = -1;
		if ( i+1 != (int)m_nodes.size() && pipe(pip) < 0) {
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
			if(e != 0)
				return e;
		}
	}
	return 0;
}
