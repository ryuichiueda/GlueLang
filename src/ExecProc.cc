// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "ExecProc.h"
//#include "ArgCommand.h"
#include "InternalCommands.h"
#include "Environment.h"
#include "Script.h"
#include "Arg.h"
#include "Where.h"
#include "DefFile.h"
#include "DefStr.h"
#include "ArgProc.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
using namespace std;

ExecProc::ExecProc(Feeder *f, Environment *env,vector<int> *scopes) : Executable(f,env,scopes)
{
}

ExecProc::~ExecProc()
{
}

bool ExecProc::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(!add(new ArgProc(m_feeder,m_env,&m_scopes)))
		return false;

	if(!m_feeder->comment() && !m_feeder->atNewLine()){
		m_feeder->blank();
		parseArgs();
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void ExecProc::execChild(DefFile *f,  DefStr *s)
{
	auto argv = makeArgv();
	// argv[0]: script file
	// argv[1,2,...]: args
	
	vOptProc(argv[0]);
	ifstream ifs(argv[0]);
	Feeder feeder(&ifs);

	m_env->initExecProc((const char**)argv);
	Script scr(&feeder,m_env,NULL);

	scr.parse();
	scr.exec(f,s); // exit in the exec function
}
