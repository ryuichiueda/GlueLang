// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "ArrayVariable.h"
#include "Feeder.h"
#include "Environment.h"
using namespace std;

ArrayVariable::ArrayVariable(Feeder *f,Environment *env) : Arg(f,env)
{
}

ArrayVariable::~ArrayVariable()
{
}

bool ArrayVariable::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	try{
		if(!m_feeder->arrayElem(&m_text,&m_array_pos))
			return false;

	}catch(Feeder *e){
		m_feeder->getPos(&m_end_line, &m_end_char);
		m_error_msg = e->m_error_msg;
		m_exit_status = 3;
		throw this;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

bool ArrayVariable::eval(void)
{
	if(m_text != "argv"){
		// treat as a literal until array can be declared in scripts.
		m_evaled_text = m_text + "[" + to_string(m_array_pos) + "]";
		return true;
	}

	try{
		m_evaled_text = *m_env->getArg(m_array_pos);
	}catch(Environment *e){
		m_error_msg = e->m_error_msg;
		m_exit_status = 3;
		throw this;
	}
	return true;
}

