// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "Element.h"
#include "Feeder.h"
#include "Environment.h"
#include <stdio.h>
#include <iostream>
using namespace std;

int Element::m_signal = 0;

Element::Element(Feeder *f, Environment *env)
{
	m_outfile = NULL;
	m_outstr = NULL;
	m_feeder = f;	
	m_env = env;

	m_start_line = m_start_char = -1000;
	m_end_line = m_end_char = -1000;
	m_exit_status = 0;

	m_job_id = 0;
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
	m_feeder->printPart(m_start_line,m_start_char,m_end_line,m_end_char);
}

string Element::pos(void)
{
	return "line " + to_string(m_start_line+1)	
		+ ", char " + to_string(m_start_char+1);
}

void Element::printErrorPart(void)
{
	m_feeder->printErrorPart(m_start_line,m_start_char,m_end_line,m_end_char);
}

int Element::getLevel(void)
{
	return m_env->getLevel();
}

void Element::setJobId(int id)
{
	m_job_id = id;
	for(auto &n : m_nodes){
		n->m_job_id = id;
		n->setJobId(id);
	}
}
