#include "Script.h"
#include "Command.h"
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

bool Script::parse(void)
{
	while(1){
		// comments -> command -> comments -> command ...
		while(1){
			Comment *cmt = new Comment(m_feeder);
			if( ! cmt->parse() ){
				delete cmt;
				break;
			}
		}

		Command *cmd = new Command(m_feeder);
		if( ! cmd->parse() ){
			delete cmd;

			if(m_error_messages.size() != 0)
				return false;

			break;
		}
		m_nodes.push_back(cmd);
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


