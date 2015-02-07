#include "StringProc.h"
#include "Command.h"
#include "InternalCommands.h"
#include "Environment.h"
#include "Arg.h"
#include "Where.h"
#include "TmpFile.h"
#include "VarString.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
using namespace std;

StringProc::StringProc(Feeder *f, Environment *env) : Element(f,env)
{
	m_pipe[0] = -1;
	m_pipe[1] = -1;
	m_pipe_prev = -1;

	m_if = false;
	m_is_wait = false;
}

StringProc::~StringProc()
{
}

/* parse of command line, where command line means
 * the combination of one command, args, and where.

	m_nodes: command arg arg ...
*/
bool StringProc::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	try{
		if(! m_feeder->literal(&m_text)){
			return false;
		}
	}catch(Feeder *e){
		m_feeder->getPos(&m_end_line, &m_end_char);
		m_error_msg = e->m_error_msg;
		m_exit_status = 1;
		throw this;
	}
	
	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

int StringProc::exec(void)
{
	cout << m_text << endl;
	return 0;
}

bool StringProc::eval(void)
{
	return true;
}
