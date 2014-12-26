#include "CommandLine.h"
#include "Command.h"
#include "Arg.h"
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
	m_file_to_write = "";
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
	//file
	string filename;
	if(m_feeder->tmpFile(&filename)){
		string pid = to_string(getpid());
		m_file_to_write = "/tmp/" + pid + "-" + filename;
		m_feeder->setVariable(&filename,&m_file_to_write);
		m_feeder->setFileList(&m_file_to_write);
	}

/*
	string com;
	add(new CommandLine(m_feeder));
*/
	if(!add(new Command(m_feeder)))
		return false;

//	cerr << ((Command *)m_nodes[0])->getStr() << endl;
//	exit(1);

	string tmp;
	m_feeder->blank(&tmp);
	

	while(add(new Arg(m_feeder))){
		m_feeder->blank(&tmp);
		if(m_feeder->atNewLine())
			return true;
/*
		if(tmp.length() == 0)
			return true;
*/
	}

	return true;
}

int CommandLine::exec(void)
{
	cout << flush;

	int pid = fork();
	if(pid < 0)
		exit(1);

	if (pid == 0){//child
		if(m_is_piped){
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

		execCommandLine();
		_exit(127);
	}

	/*parent*/

	if(m_is_piped){
		if(m_pipe_prev >= 0)
			close(m_pipe_prev);

		m_pipe_prev = m_pipe[0];
		close(m_pipe[1]);
	}

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

bool CommandLine::setRedirectTo(void)
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

void CommandLine::execCommandLine(void)
{
	//The child process should not access to the source code.
	m_feeder->close();

	if(m_file_to_write != ""){
		if(! setRedirectTo() )
			return;
	}

	auto **argv = new const char* [m_nodes.size()];
	argv[0] = ((Command *)m_nodes[0])->getStr();
	for (int i=1;i < (int)m_nodes.size();i++){
		m_nodes[i]->eval();
		argv[i] = ((Arg *)m_nodes[i])->getEvaledString();
	}

	argv[m_nodes.size()] = NULL;

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
