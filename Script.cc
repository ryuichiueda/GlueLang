#include "Script.h"
#include "Command.h"
using namespace std;

Script::Script(Feeder *f) : Node(f)
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
	while(1){
		Command *c = new Command(m_feeder);
		if( ! c->parse() ){
			delete c;
			break;
		}
		m_nodes.push_back(c);
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
