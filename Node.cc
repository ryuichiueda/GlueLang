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
