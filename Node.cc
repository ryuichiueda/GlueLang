#include "Node.h"
#include <iostream>
using namespace std;

Node::Node(Feeder *f)
{
//	m_set = false;
	m_feeder = f;	
}

Node::~Node()
{
/*
	for(auto p : m_node){
		cerr << "!" << endl;
		delete p;
	}
*/
}

/*
void Node::print(int indent_level) 
{
	for(auto &p : m_node){
		p->print(indent_level++);
	}
}

Node *Node::getNode(int at)
{
	return m_node.at(at);
}

void Node::pushNode(Node *n)
{
	m_node.push_back(n);
}


*/

bool Node::isNum(char c)
{
	if(c >= '0' && c <= '9')
		return true;

	return false;
}

bool Node::isAlphabet(char c)
{
	if(c >= 'a' && c <= 'z')
		return true;
	if(c >= 'A' && c <= 'Z')
		return true;

	return false;
}

void Node::printErrorMessages(void)
{
	for(auto e : m_error_messages){
		cerr << e << endl;
	}
}
