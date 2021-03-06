// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "ArgIntCom.h"
#include "Feeder.h"
#include "Environment.h"
#include "InternalCommands.h"
using namespace std;

ArgIntCom::ArgIntCom(Feeder *f,Environment *env,vector<int> *scopes) : Arg(f,env,scopes)
{
}

ArgIntCom::~ArgIntCom()
{
}

bool ArgIntCom::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	m_feeder->str("in.");

	string tmp;
	if(!m_feeder->variable(&m_evaled_text)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_text = "in." + m_evaled_text;

	if(m_evaled_text != "wait" && !InternalCommands::exist(&m_evaled_text)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}


	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

/*
bool ArgIntCom::eval(void)
{
	return true;
}
*/

