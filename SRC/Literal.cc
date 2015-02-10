#include "Literal.h"
#include "Feeder.h"
#include "Arg.h"
#include "Environment.h"
using namespace std;

Literal::Literal(Feeder *f,Environment *env) : Arg(f,env)
{
}

Literal::~Literal()
{
}

bool Literal::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	try{
		if( m_feeder->literal(&m_text)){
			m_feeder->getPos(&m_end_line, &m_end_char);
			return true;
		}
	}catch(Feeder *e){
		m_feeder->getPos(&m_end_line, &m_end_char);
		m_error_msg = e->m_error_msg;
		m_exit_status = 1;
		throw this;
	}

	return false;
}

bool Literal::eval(void)
{
	m_evaled_text = m_text;
	//evaled escaped characters 
	auto pos = m_evaled_text.find("\\\'"); // \'
	while(pos != string::npos){
		m_evaled_text.replace(pos, 2, "\'");
		pos = m_evaled_text.find("\\\'", pos + 1);
	}
	return true;
}
