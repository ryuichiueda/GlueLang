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

Command::Command(Feeder *f) : Node(f)
{
	m_file_to_write = "";
}

Command::~Command()
{

}

void Command::print(int indent_level)
{
	cout << m_name << endl;
}

void Command::setName(string s){
	m_name = s;
	Arg arg(&m_name,m_feeder);
	m_args.push_back(arg);
}

void Command::appendArg(string a){
	Arg arg(&a,m_feeder);
	m_args.push_back(arg);
}

// file f = command ...
// command ...
bool Command::parse(void)
{
	//file
	string filename;
	if(m_feeder->getTmpFile(&filename)){
		string pid = to_string(getpid());
		m_file_to_write = "/tmp/" + pid + "-" + filename;
	}
	
	//command
	string com,arg;
	if(! m_feeder->getCommand(&com))
		return false;

	setName(com);

	while(!m_feeder->atNewLine()){
		m_feeder->getArg(&arg);
		appendArg(arg);
	}

	return true;
}

int Command::exec(void)
{
	cout << flush;

	int pid = fork();
	if(pid < 0)/* error */
		exit(1);

	if (pid == 0){/* child */
		execCommand();
		_exit(127);
	}

	/* parent */
	int status;
	int options = 0;
	waitpid(pid,&status,options);

	if(WIFEXITED(status)){
		return WEXITSTATUS(status);
	}

	return -1;
}

bool Command::setRedirectTo(void)
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

void Command::execCommand(void)
{
	//The child process should not access to the source code.
	m_feeder->close();

	if(m_file_to_write != ""){
		if(! setRedirectTo() )
			return;
	}

	auto **argv = new const char* [m_args.size()];
	for (int i=0;i < m_args.size();i++){
		m_args[i].eval();
		argv[i] = m_args[i].getEvaledString();
	}

	argv[m_args.size()] = NULL;

	execve(argv[0],(char **)argv,NULL);
}

