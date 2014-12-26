#include "Element.h"
#include "Feeder.h"
#include "Script.h"
#include "Pipeline.h"
#include "CommandLine.h"
#include "Comment.h"
using namespace std;

Script::Script(Feeder *f) : Element(f)
{
}

Script::~Script()
{
	for(auto &n : m_nodes){
		delete n;
	}
}

/*
bool Script::tryElement(Element *n)
{
	if( n->parse() ){
		m_nodes.push_back(n);
		return true;
	}
	delete n;
	return false;
}
*/

bool Script::parse(void)
{
	while(1){
		// comments -> pipeline or command -> comments -> pipeline or command ...
		bool repeat = false;
		while(add(new Comment(m_feeder))){
		}

		if(add(new Pipeline(m_feeder))){
			repeat = true;
		}else if(m_error_messages.size() != 0){
			return false;
		}

		if(add(new CommandLine(m_feeder))){
			repeat = true;
		}else if(m_error_messages.size() != 0){
			return false;
		}else{
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


