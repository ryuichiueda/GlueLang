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

	if(!m_feeder->str("this."))
		return false;

	string tmp;
	if(!m_feeder->variable(&m_evaled_text))
		return false;

	m_text = "this." + m_evaled_text;
	m_evaled_text = m_env->m_tmpdir + "/" + m_evaled_text;
	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

bool ArgProc::eval(void)
{
	return true;
}

