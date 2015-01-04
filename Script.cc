#include "Element.h"
#include "Feeder.h"
#include "Script.h"
#include "Import.h"
#include "IfBlock.h"
#include "Pipeline.h"
#include "CommandLine.h"
#include "Comment.h"
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
	}

	while(1){
		// comments -> pipeline or command -> comments -> pipeline or command ...
		m_feeder->blankLines();
		if(m_feeder->atEnd())
			break;

		while(add(new Comment(m_feeder,m_env))){ }

		if(	add(new IfBlock(m_feeder,m_env))
			|| add(new Pipeline(m_feeder,m_env))
		 	|| add(new CommandLine(m_feeder,m_env))){

			if(m_feeder->atEnd())
				break;
		
			continue;
		}

		break;
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
