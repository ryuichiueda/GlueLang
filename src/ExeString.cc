// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "ExeString.h"
//#include "ArgCommand.h"
#include "InternalCommands.h"
#include "Environment.h"
#include "Script.h"
#include "Arg.h"
#include "Literal.h"
#include "ArgVariable.h"
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

ExeString::ExeString(Feeder *f, Environment *env, vector<int> *scopes) : Exe(f,env,scopes)
{
}

ExeString::~ExeString()
{
}

bool ExeString::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	bool res = add(new Literal(m_feeder,m_env,&m_scopes));

	if(!res)
		return false;

	while(res){
		if(m_feeder->atNewLine())
			break;
		res = add(new Literal(m_feeder,m_env,&m_scopes))
			|| add(new ArgVariable(m_feeder,m_env,&m_scopes));
	}
	
	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

void ExeString::execChild(DefFile *f, DefStr *s)
{
	string str,tmp;
	for(auto n : m_nodes){
		tmp = ((Arg *)n)->getEvaledString();
		str += tmp;
	}

	const char **argv = new const char *[3];
	string com = "echo";
	argv[0] = (const char *)com.c_str();
	argv[1] = (const char *)str.c_str();
	argv[2] = NULL;
	vOptProc(argv[0]);
	InternalCommands::exec(argv,m_env,m_feeder,this);
}
