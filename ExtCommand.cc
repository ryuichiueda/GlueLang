#include "ExtCommand.h"
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

ExtCommand::ExtCommand(Feeder *f, Environment *env) : CommandLine(f,env)
{
}

ExtCommand::~ExtCommand()
{
}

bool ExtCommand::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(!add(new ArgCommand(m_feeder,m_env)))
		return false;

	if(!m_feeder->comment() && !m_feeder->atNewLine()){
		m_feeder->blank();
		parseArgs();
	}

	auto *c = (ArgCommand *)m_nodes[0];
	if(c->m_is_internal && c->m_name == "wait"){
		m_is_wait = true;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void ExtCommand::execChild(void)
{
	auto argv = makeArgv();
	vOptProc(argv[0]);
	execv(argv[0],(char **)argv);
}

