#include "Node.h"
#include <stdio.h>
#include <iostream>
using namespace std;

Node::Node(Feeder *f)
{
	m_feeder = f;	
}

Node::~Node()
{
}

void Node::printErrorMessages(void)
{
	for(auto e : m_error_messages){
		cerr << e << endl;
	}
}
