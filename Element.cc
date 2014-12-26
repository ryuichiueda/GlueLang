#include "Element.h"
#include <stdio.h>
#include <iostream>
using namespace std;

Element::Element(Feeder *f)
{
	m_feeder = f;	
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
