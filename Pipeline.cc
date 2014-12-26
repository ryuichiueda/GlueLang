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

Pipeline::Pipeline(Feeder *f) : Element(f)
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
	while(1){
		bool repeat = false;

		if(add(new CommandLine(m_feeder))){
			repeat = true;
		}else if(m_error_messages.size() != 0){
			return false;
		}else{
			return true;
		}
	
		if(! repeat)
			break;
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

	int pid = fork();
	if(pid < 0)/* error */
		exit(1);

	if (pid == 0){/* child */
		execPipeline();
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

