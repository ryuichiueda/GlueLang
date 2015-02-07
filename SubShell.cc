#include "SubShell.h"
#include "ArgCommand.h"
#include "InternalCommands.h"
#include "Environment.h"
#include "Script.h"
#include "Arg.h"
#include "StringArray.h"
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
#include "GlueString.h"
using namespace std;

SubShell::SubShell(Feeder *f, Environment *env) : CommandLine(f,env)
{
}

SubShell::~SubShell()
{
}

bool SubShell::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);
	if(!m_feeder->str("this.")){
		return false;
	}
	//rewind
	m_feeder->setPos(m_start_line, m_start_char);

	// start from a command
	if(!add(new ArgCommand(m_feeder,m_env)))
		return false;

	if(!m_feeder->comment() && !m_feeder->atNewLine()){
		m_feeder->blank();
		parseArgs();
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void SubShell::execChild(void)
{
	auto argv = makeArgv();
	// argv[0]: script file
	// argv[1,2,...]: args
	
	vOptProc(argv[0]);
	ifstream ifs(argv[0]);
	Feeder feeder(&ifs);

	m_env->initSubShell(argv);
	Script s(&feeder,m_env);

	s.parse();
	s.exec(); // exit in the exec function
}
