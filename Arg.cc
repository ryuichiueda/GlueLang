#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
using namespace std;

Arg::Arg(Feeder *f,Environment *env) : Element(f,env)
{
	m_is_variable = false;
	m_is_array_variable = false;

	//m_is_local_variable = false;
}

Arg::~Arg()
{
}

bool Arg::eval(void)
{
	if(m_is_array_variable){
		if(m_text == "argv"){
			try{
				m_evaled_text = *m_env->getArg(m_array_pos);
			}catch(Environment *e){
				m_error_msg = e->m_error_msg;
				m_exit_status = 1;
				throw this;
			}
		}else // treat as a literal until array can be declared in scripts.
			m_evaled_text = m_text + "[" + to_string(m_array_pos) + "]";
		return true;
	}

	if(m_is_variable){
		try{
			m_env->getVariable(&m_text,&m_evaled_text);
		}catch(...){
			m_error_msg = m_env->m_error_msg;
			m_exit_status = 1;
			throw this;
		}
		return true;
	}

	m_evaled_text = m_text;
	//evaled escaped characters 
	auto pos = m_evaled_text.find("\\\'"); // \'
	while(pos != string::npos){
		m_evaled_text.replace(pos, 2, "\'");
		pos = m_evaled_text.find("\\\'", pos + 1);
	}
	return true;
}

int Arg::exec(void)
{
	return 0;
}

bool Arg::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	try{
		if( m_feeder->arrayElem(&m_text,&m_array_pos)){
			m_is_array_variable = true;
			m_feeder->getPos(&m_end_line, &m_end_char);
			return true;
		}
	}catch(Feeder *e){
		m_feeder->getPos(&m_end_line, &m_end_char);
		m_error_msg = e->m_error_msg;
		m_exit_status = 1;
		throw this;
	}

	try{
		if( m_feeder->literal(&m_text)){
			m_is_variable = false;
			m_feeder->getPos(&m_end_line, &m_end_char);
			return true;
		}
	}catch(Feeder *e){
		m_feeder->getPos(&m_end_line, &m_end_char);
		m_error_msg = e->m_error_msg;
		m_exit_status = 1;
		throw this;
	}

	if(m_feeder->variable(&m_text)){
		m_is_variable = true;
		m_feeder->getPos(&m_end_line, &m_end_char);
		return true;
	}

	return false;
}
