#include "Element.h"
#include <stdio.h>
#include <iostream>
using namespace std;

Element::Element(Feeder *f, Environment *env)
{
	m_feeder = f;	
	m_env = env;
}

Element::~Element()
{
}

bool Element::add(Element *n)
{
	if( n->parse() ){
		m_nodes.push_back(n);
		return true;
	}
	delete n;
	return false;
}

void Element::printOriginalString(void)
{
	for(auto s : m_nodes){
		s->printOriginalString();
	}
	cerr << flush;
}

/*
void Element::errorCheck(void)
{
	if(m_error_messages.size() == 0)
		return;

	for(auto e : m_error_messages){
		cerr << e << endl;
	}
	exit(1);
}
*/
