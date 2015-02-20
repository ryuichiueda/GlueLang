#include "IntCommand.h"
#include "ArgIntCommand.h"
#include "InternalCommands.h"
#include "Environment.h"
#include "Script.h"
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

IntCommand::IntCommand(Feeder *f, Environment *env) : CommandLine(f,env)
{
}

IntCommand::~IntCommand()
{
}

bool IntCommand::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);
/*
	if(!m_feeder->str("in.")){
		return false;
	}
	//rewind
	m_feeder->setPos(m_start_line, m_start_char);
*/

	if(!add(new ArgIntCommand(m_feeder,m_env)))
		return false;
/*
	// start from a command
	if(!add(new ArgIntCommand(m_feeder,m_env)))
		return false;
*/

	if(!m_feeder->comment() && !m_feeder->atNewLine()){
		m_feeder->blank();
		parseArgs();
	}

	auto *c = (ArgIntCommand *)m_nodes[0];
	if(c->m_text == "in.wait"){
		m_is_wait = true;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void IntCommand::execChild(void)
{
	auto argv = makeArgv();
	vOptProc(argv[0]);
	InternalCommands::exec((const char**)argv,m_env,m_feeder,this);
}
