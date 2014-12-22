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
}

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
