#include "Command.h"
#include "Arg.h"
using namespace std;

Command::Command(Feeder *f) : Node(f)
{
//	parser(script,pos);
}

Command::~Command()
{

}

void Command::print(int indent_level)
{

}

void Command::parser(vector<char> *script, int pos)
{
/*
	m_node.push_back(new Arg(script,pos));
	if(!m_node.back()->isSet()){
		delete m_node.back();
		m_node.pop_back();
	}else{
		m_node.back()->print();
	}

	m_set = m_node.size() != 0;
*/
}

