#include "CommandLine.h"
#include "Command.h"
#include "Arg.h"
#include "TmpFile.h"
#include "VarString.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
using namespace std;

CommandLine::CommandLine(Feeder *f, Environment *env) : Element(f,env)
{
	m_outfile = NULL;
	m_outstr = NULL;
	m_pipe[0] = -1;
	m_pipe[1] = -1;
	m_pipe_prev = -1;
	m_is_piped = false;
}

CommandLine::~CommandLine()
{

}

/* parse of command line, where command line means
 * the combination of one command, args, and files.
 * We assume two patterns of commandline now:

	file f = command ...
	command ...

* to do:
	to implement file redirection for standard error, like
	file f1 f2 = command ... 
*/
bool CommandLine::parse(void)
{
	if(add(new TmpFile(m_feeder,m_env)))
		m_outfile = (TmpFile *)m_nodes[0];
	else if(add(new VarString(m_feeder,m_env)))
		m_outstr = (VarString *)m_nodes[0];

	if(!add(new Command(m_feeder,m_env)))
		return false;


	m_feeder->blank(NULL);

	while(add(new Arg(m_feeder,m_env))){
		m_feeder->blank(NULL);
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

	if(m_pipe[1] >= 0)
		close(m_pipe[0]);

	if(m_pipe_prev > 0) {
		dup2(m_pipe_prev, 0);
		close(m_pipe_prev);
	}
	if(m_pipe[1] > 1){
		dup2(m_pipe[1], 1);
		close(m_pipe[1]);
	}
}

void CommandLine::execErrorExit(void)
{
	perror("ERROR: exec() failed");
	cerr << "Failed to execute command: ";
	printOriginalString();
	_exit(127);
}

int CommandLine::exec(void)
{
	cout << flush;

	if(! eval()){
		return -1;
	}

	int pid = fork();
	if(pid < 0)
		exit(1);

	if (pid == 0){//child
		childPipeProc();
		execCommandLine();
		execErrorExit();
	}

	/*parent*/
	if(m_is_piped){
		parentPipeProc();
		return pid;
	}

	if(m_outstr != NULL){
		m_outstr->readFiFo();
	}

	int status;
	int options = 0;
	waitpid(pid,&status,options);

	if(WIFEXITED(status)){
		return WEXITSTATUS(status);
	}

	return -1;
}

const char** CommandLine::makeArgv(int file_num)
{
	auto argv = new const char* [m_nodes.size() - file_num];
	argv[0] = ((Command *)m_nodes[file_num])->getStr();
	for (int i=1;i < (int)m_nodes.size()-file_num;i++){
		argv[i] = ((Arg *)m_nodes[file_num+i])->getEvaledString();
	}

	argv[m_nodes.size()-file_num] = NULL;
	return argv;
}

void CommandLine::execCommandLine(void)
{
	//The child process should not access to the source code.
	m_feeder->close();

	int io_num = 0;
	if(m_outfile != NULL){
		if(m_outfile->exec() != 0)
			return;

		io_num++;
	}else if(m_outstr != NULL){
		if(m_outstr->exec() != 0)
			return;

		io_num++;
	}

	auto argv = makeArgv(io_num);
	execve(argv[0],(char **)argv,NULL);
}

bool CommandLine::eval(void)
{
	for(auto s : m_nodes){
		if( ! s->eval() ){
			m_error_messages.push_back("evaluation of args failed");
			return false;
		}
	}
	return true;
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

void CommandLine::pushOutFile(TmpFile *e)
{
	m_nodes.insert(m_nodes.begin(),e);
	m_outfile = e;
}

void CommandLine::pushVarString(VarString *e)
{
	m_nodes.insert(m_nodes.begin(),e);
	m_outstr = e;
}
