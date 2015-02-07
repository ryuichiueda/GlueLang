#include "Element.h"
#include "Feeder.h"
#include "Script.h"
#include "Import.h"
#include "Procedure.h"
#include "IfBlock.h"
#include "Pipeline.h"
#include "Job.h"
#include "CommandLine.h"
#include "Environment.h"
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
using namespace std;

Script::Script(Feeder *f, Environment *env) : Element(f,env)
{
}

Script::~Script()
{
	for(auto &n : m_nodes){
		delete n;
	}
}

bool Script::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	// import
	while(add(new Import(m_feeder,m_env))){
		while(m_feeder->blankLine());
	}

	m_env->initTmpdir();

	while(1){
		// comments -> proc or and -> proc or and -> ...

		while(m_feeder->comment());

		if( add(new Procedure(m_feeder,m_env))){
		}else if( add(new Job(m_feeder,m_env))){
		}else if( add(new IfBlock(m_feeder,m_env))){
		}else{
			break;
		}
	
		while(m_feeder->blankLine());
		continue;
	}
	m_feeder->getPos(&m_end_line, &m_end_char);

	if(!m_feeder->atEnd()){
		m_error_msg = "Unknown token";
		m_exit_status = 1;
		throw this;
	}

	return true;
}

int Script::exec(void)
{
	int exit_status = 0;
	for(auto &c : m_nodes){
		exit_status = c->exec();
	}
	return exit_status;
}
