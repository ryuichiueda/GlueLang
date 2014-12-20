#include "Script.h"
#include "Arg.h"
#include "Feeder.h"
#include "Command.h"
#include <iostream>
using namespace std;

Script::Script(Feeder *f) : Node(f)
{
	m_feeder->readLine();
	m_feeder->print();
	//parser(script,pos);
}

Script::~Script()
{
}

void Script::print(int indent_level) 
{
	for(auto &p : m_node){
		p->print(indent_level++);
	}
}

void Script::parser(vector<char> *script, int pos)
{
/*
	m_node.push_back(new Command(script,pos));
	if(!m_node.back()->isSet()){
		delete m_node.back();
		m_node.pop_back();
	}else{
		m_node.back()->print();
	}

	m_set = m_node.size() != 0;
*/
}

