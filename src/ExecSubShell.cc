// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "ExecSubShell.h"
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
#include "DataProc.h"
using namespace std;

ExecSubShell::ExecSubShell(Feeder *f, Environment *env,vector<int> *scopes) : Executable(f,env,scopes)
{
}

ExecSubShell::~ExecSubShell()
{
}

bool ExecSubShell::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	string scr;
	bool scr_exist = m_feeder->doBlock(&scr);
	if(!scr_exist){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_name = "unnamed" + to_string(m_start_line);

	//create a file
	string tmpdir = m_env->m_tmpdir + "/" + m_name;
	ofstream ofs(tmpdir.c_str());
	ofs << scr;

	auto *p = new DataProc();
	p->setFileName(&tmpdir);
	m_env->setData(m_scopes.back(),&m_name,p);
	return true;
}

void ExecSubShell::execChild(DefFile *f,  DefStr *s)
{
	auto argv = new char* [2];
	string tmpdir = m_env->m_tmpdir + "/" + m_name;
	argv[0] = (char *)tmpdir.c_str();
	argv[1] = NULL;

	vOptProc(argv[0]);
	ifstream ifs(argv[0]);
	Feeder feeder(&ifs);

	m_env->initExecProc((const char**)argv);
	Script scr(&feeder,m_env,NULL);

	scr.setSilent();
	scr.parse();
	scr.exec(f,s); // exit in the exec function
}
