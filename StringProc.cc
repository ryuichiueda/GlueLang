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
	m_com = "echo";
}

StringProc::~StringProc()
{
}

/* parse of procedures for strings
 * now this recognizes just one literal
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
	return 0;
}

bool StringProc::eval(void)
{
	return true;
}

const char** StringProc::makeArgv(void)
{
	auto argv = new const char* [m_nodes.size() + 2];
	argv[0] = m_com.c_str();
	argv[1] = m_text.c_str();
	argv[2] = NULL;
	return argv;
}
