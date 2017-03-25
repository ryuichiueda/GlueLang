// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "ArgProc.h"
#include "Feeder.h"
#include "Environment.h"
using namespace std;

ArgProc::ArgProc(Feeder *f,Environment *env) : Arg(f,env)
{
}

ArgProc::~ArgProc()
{
}

bool ArgProc::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	m_feeder->str("this.");

	string tmp;
	if(!m_feeder->variable(&m_evaled_text)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}
	if(!m_env->isData(&m_evaled_text)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_text = "this." + m_evaled_text;
	m_evaled_text = m_env->m_tmpdir + "/" + m_evaled_text;
	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

bool ArgProc::eval(void)
{
	return true;
}

