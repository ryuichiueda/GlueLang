#include "Element.h"
#include "Feeder.h"
#include "Script.h"
#include "Import.h"
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
	// import
	while(add(new Import(m_feeder,m_env))){
	}

	while(1){
		// comments -> pipeline or command -> comments -> pipeline or command ...
		bool repeat = false;
		while(add(new Comment(m_feeder,m_env))){ }

		if(add(new Pipeline(m_feeder,m_env))){
			repeat = true;
			continue;
		}else{
			errorCheck();
		}

		if(add(new CommandLine(m_feeder,m_env))){
			repeat = true;
		}else{
			errorCheck();

			return true;
		}

		if(! repeat)
			break;
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
