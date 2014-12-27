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

void Element::printErrorMessages(void)
{
	for(auto e : m_error_messages){
		cerr << e << endl;
	}
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
