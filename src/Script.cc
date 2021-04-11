// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "Element.h"
#include "Feeder.h"
#include "Script.h"
#include "Import.h"
#include "DefProc.h"
#include "Pipeline.h"
#include "Job.h"
#include "Exe.h"
#include "Environment.h"
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
using namespace std;

Script::Script(Feeder *f, Environment *env,vector<int> *scopes) : Element(f,env,scopes)
{
	m_silent = false;
}

Script::~Script()
{
	for(auto &n : m_nodes){
		if(n != NULL)
			delete n;
	}
}

bool Script::parse(void)
{
	try{
		return parseScript();
	}
	catch(Element *e){
		printErrorMsg(e,"Parse error");
		m_env->removeFiles();
		end(e->m_exit_status);
	}catch(...){
		cerr << "\nParse error"
		     << "\nunknown error" << endl;
		m_env->removeFiles();
		end(128);
	}
	return true;
}

bool Script::parseScript(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	// mainly for shebang
	while(m_feeder->comment());
	// import
	while(add(new Import(m_feeder,m_env,&m_scopes))){
		while(m_feeder->blankLine());
	}

	m_env->initTmpdir();

	while(1){
		// comments -> proc or job -> proc or job -> ...

		while(m_feeder->comment());

		bool res = add(new DefProc(m_feeder,m_env,&m_scopes))
			|| add(new Job(m_feeder,m_env,&m_scopes));

		if(!res)
			break;
	
		while(m_feeder->blankLine());
	}
	m_feeder->getPos(&m_end_line, &m_end_char);

	if(!m_feeder->atEnd()){
		m_error_msg = "Unknown token";
		m_exit_status = 7;
		throw this;
	}

	return true;
}

int Script::exec(DefFile *f, DefStr *s)
{
	int exit_status = 0;
	try{
		for(auto &c : m_nodes){
			exit_status = c->exec(NULL,NULL);
		}
		m_env->removeFiles();
		if(exit_status == 0)
			end(0);
	}catch(Element *e){
		printErrorMsg(e,"Execution error",m_silent);
		m_env->removeFiles();
		if(e->m_command_error)
			end(1);
		else
			end(e->getExitStatus());
	}catch(...){
		cerr << "\nExecution error\n"
		     << "unknown error" << endl;
		m_env->removeFiles();
		end(3);
	}
	cerr << "unknown error (uncatched)" << endl;
	m_env->removeFiles();
	end(3);
	return 0;
}

void Script::printErrorMsg(Element *e,string error_type, bool silent)
{
	if(silent)
		return;

	cerr << error_type << " at " << e->pos() << endl;
	e->printErrorPart();
	cerr << "\n\t" << e->m_error_msg
	     << "\n\tprocess_level " << e->getLevel()
	     << "\n\texit_status " << e->getExitStatus()
	     << "\n\tpid " << getpid() << endl;
}

void Script::end(int exit_status)
{
	if(exit_status != 0 and m_env->m_level == 0)
		cerr << "\e[31mERROR: " << exit_status << "\e[m" << endl;
	exit(exit_status);
}

