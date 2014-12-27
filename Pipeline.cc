#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "Pipeline.h"
#include "CommandLine.h"
#include "Arg.h"
#include "Feeder.h"
using namespace std;

Pipeline::Pipeline(Feeder *f, Environment *env) : Element(f,env)
{
	m_file_to_write = "";
}

Pipeline::~Pipeline()
{

}

void Pipeline::print(int indent_level)
{
}

// file f = command ...
// command ...
bool Pipeline::parse(void)
{
	int prev_ln,prev_ch;
	m_feeder->getCurPos(&prev_ln, &prev_ch);

	int comnum = 0;
	string tmp;
	while(1){
		bool repeat = false;

		if(add(new CommandLine(m_feeder,m_env))){
			repeat = true;
			comnum++;
		}else if(m_error_messages.size() != 0){
			return false; // error exit
		}else{
			break;
		}

		if(! m_feeder->pipe(&tmp)){
			break;
		}

		m_feeder->blank(&tmp);

		if(! repeat)
			break;
	}

	if(comnum < 2 /*|| ! m_feeder->pipeEnd(&tmp)*/ ){
		m_feeder->rewind(prev_ln,prev_ch);
		return false;
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
	int n = 1;
	for(auto c : m_nodes){
		auto *p = (CommandLine *)c;
		//p->printOriginalString();
		pip[1] = -1;
		if (n != m_nodes.size() && pipe(pip) < 0) {
			close(prevfd);
			m_error_messages.push_back("Pipe call failed");
		}
		p->setPipe(pip,prevfd);
		pids.push_back( p->exec() );
		prevfd = p->getPrevPipe();

		n++;
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

bool Pipeline::setRedirectTo(void)
{
	int fd = open(m_file_to_write.c_str(),O_WRONLY | O_CREAT,0700);
	if(fd < 3){
		m_error_messages.push_back(
			"file: " + m_file_to_write + " does not open.");
		return false;
	}
	if(dup2(fd,1) < 0){
		m_error_messages.push_back(
			"file: " + m_file_to_write + "  redirect error");
		return false;
	}
	if( close(fd) < 0){
		m_error_messages.push_back(
			"file: " + m_file_to_write + "  redirect error");
		return false;
	}

	return true;
}

void Pipeline::execPipeline(void)
{
}

