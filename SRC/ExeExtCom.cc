#include "ExeExtCom.h"
#include "ArgExtCom.h"
#include "InternalCommands.h"
#include "Environment.h"
#include "Script.h"
#include "Arg.h"
#include "Where.h"
#include "DefFile.h"
#include "DefStr.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
using namespace std;

ExeExtCom::ExeExtCom(Feeder *f, Environment *env) : Exe(f,env)
{
}

ExeExtCom::~ExeExtCom()
{
}

bool ExeExtCom::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(!add(new ArgExtCom(m_feeder,m_env)))
		return false;

	if(!m_feeder->comment() && !m_feeder->atNewLine()){
		m_feeder->blank();
		parseArgs();
	}


	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void ExeExtCom::execChild(void)
{
	auto argv = makeArgv();
	vOptProc(argv[0]);
	char *str = getenv("PATH");
	auto env = new const char* [2];
	env[0] = str;
	env[1] = NULL;
	execve(argv[0],(char **)argv,(char **)env);
}

