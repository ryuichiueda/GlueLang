// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "ArgVariable.h"
#include "Feeder.h"
#include "Environment.h"
#include "DataProc.h"
using namespace std;

ArgVariable::ArgVariable(Feeder *f,Environment *env) : Arg(f,env)
{
	m_is_local = false;
	m_is_proc = false;
}

ArgVariable::~ArgVariable()
{
}

bool ArgVariable::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(m_feeder->str("local.")){
		m_is_local = true;
	}else if(m_feeder->str("this.")){
		m_is_proc = true;
	}

	if(!m_feeder->variable(&m_text))
		return false;

	if(m_is_local)
		m_text = "local." + m_text;
//	else if(m_is_proc)
//	m_text = "this." + m_text;

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

bool ArgVariable::eval(void)
{
	try{
		Data *p;
		if(m_is_local){
			p = m_env->m_local_env[m_job_id]->getData(&m_text);
		}else{
			p = m_env->getData(&m_text);
		}
		m_evaled_text = p->m_value;
	}catch(...){
		m_error_msg = m_env->m_error_msg;
		m_exit_status = 1;
		throw this;
	}
	return true;
}

