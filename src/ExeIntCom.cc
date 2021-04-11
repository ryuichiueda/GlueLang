// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "ExeIntCom.h"
#include "ArgIntCom.h"
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

ExeIntCom::ExeIntCom(Feeder *f, Environment *env,vector<int> *scopes) : Exe(f,env,scopes)
{
}

ExeIntCom::~ExeIntCom()
{
}

bool ExeIntCom::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(!add(new ArgIntCom(m_feeder,m_env,&m_scopes)))
		return false;

	if(!m_feeder->comment() && !m_feeder->atNewLine()){
		m_feeder->blank();
		parseArgs();
	}

	auto *c = (ArgIntCom *)m_nodes[0];
	if(c->m_text == "in.wait"){
		m_is_wait = true;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void ExeIntCom::execChild(DefFile *f, DefStr *s)
{
	auto argv = makeArgv();
	vOptProc(argv[0]);
	InternalCommands::exec((const char**)argv,m_env,m_feeder,this);
}
