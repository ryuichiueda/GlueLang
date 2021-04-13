// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "DefCond.h"
#include "Feeder.h"
#include "Environment.h"
#include <sys/stat.h>
using namespace std;

DefCond::DefCond(Feeder *f,Environment *env, vector<int> *scopes) : Element(f,env,scopes)
{
	m_upper_byte = -1;
}

DefCond::~DefCond()
{
}

/*
 * DefCond objects give and check some restrictions to files
 * and strings.
 */
bool DefCond::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(!m_feeder->str("cond"))
		return false;

	if(! m_feeder->variable(&m_name)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	if(! m_feeder->str(":")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	if(!m_feeder->str("length")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	if(!m_feeder->str("<")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}


	if(!m_feeder->positiveInt(&m_upper_byte)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

