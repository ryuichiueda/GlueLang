#include "CommandLine.h"
#include "Command.h"
#include "Arg.h"
#include "TmpFile.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
using namespace std;

CommandLine::CommandLine(Feeder *f) : Element(f)
{
	m_file_to_write = false;
	m_pipe[0] = -1;
	m_pipe[1] = -1;
	m_pipe_prev = -1;
	m_is_piped = false;
}

CommandLine::~CommandLine()
{

}

void CommandLine::print(int indent_level)
{
}

/*
void CommandLine::setName(string s){
	//m_name = s;
	Arg arg(&m_name,m_feeder);
	//m_args.push_back(arg);
	m_nodes.push_back(arg);
}
*/

/*
void CommandLine::appendArg(string a){
	Arg arg(&a,m_feeder);
	m_args.push_back(arg);
}
*/

// file f = command ...
// command ...
bool CommandLine::parse(void)
{
	if(add(new TmpFile(m_feeder)))
		m_file_to_write = true;

	if(!add(new Command(m_feeder)))
		return false;

	string tmp;
	m_feeder->blank(&tmp);

	while(add(new Arg(m_feeder))){
		m_feeder->blank(&tmp);
		if(m_feeder->atNewLine())
			return true;
	}


	return true;
}

void CommandLine::parentPipeProc(void)
{
	if(!m_is_piped)
		return;

	if(m_pipe_prev >= 0)
		close(m_pipe_prev);

	m_pipe_prev = m_pipe[0];
	close(m_pipe[1]);
}

void CommandLine::childPipeProc(void)
{
	if(!m_is_piped)
		return;

	if(m_pipe[1] >= 0){
		close(m_pipe[0]);
	}
	if(m_pipe_prev > 0) {
		dup2(m_pipe_prev, 0);
		close(m_pipe_prev);
	}
	if(m_pipe[1] > 1){
		dup2(m_pipe[1], 1);
		close(m_pipe[1]);
	}
}

int CommandLine::exec(void)
{
	cout << flush;

	int pid = fork();
	if(pid < 0)
		exit(1);

	if (pid == 0){//child
		childPipeProc();
		execCommandLine();
		_exit(127);
	}

	/*parent*/
	parentPipeProc();

	int status;
	int options = 0;

	if(m_is_piped)
		return pid;

	waitpid(pid,&status,options);

	if(WIFEXITED(status)){
		return WEXITSTATUS(status);
	}

	return -1;
}

bool CommandLine::setRedirectTo(TmpFile *f)
{
	int fd = open( f->actualFileName() ,O_WRONLY | O_CREAT,0700);
	if(fd < 3){
		m_error_messages.push_back(
			"file: " + string(f->virtualFileName()) + " does not open.");
		return false;
	}
	if(dup2(fd,1) < 0){
		m_error_messages.push_back(
			"file: " + string(f->virtualFileName()) + "  redirect error");
		return false;
	}
	if( close(fd) < 0){
		m_error_messages.push_back(
			"file: " + string(f->virtualFileName()) + "  redirect error");
		return false;
	}

	return true;
}

void CommandLine::execCommandLine(void)
{
	//The child process should not access to the source code.
	m_feeder->close();

	int org = 0;
	if(m_file_to_write == true){
		org++;
		if(! setRedirectTo((TmpFile *)m_nodes[0]) )
			return;
	}

	auto **argv = new const char* [m_nodes.size()-org];
	argv[0] = ((Command *)m_nodes[org])->getStr();
	for (int i=1;i < (int)m_nodes.size()-org;i++){
		m_nodes[org+i]->eval();
		argv[i] = ((Arg *)m_nodes[org+i])->getEvaledString();
	}

	argv[m_nodes.size()-org] = NULL;

	execve(argv[0],(char **)argv,NULL);
}

void CommandLine::printOriginalString(void)
{
	for(auto s : m_nodes){
		s->printOriginalString();
	}
	cerr << endl;
}

void CommandLine::setPipe(int *pip,int prev)
{
	m_pipe[0] = pip[0];
	m_pipe[1] = pip[1];
	m_pipe_prev = prev;
	m_is_piped = true;
}
