#include "VarString.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "Environment.h"
#include "Condition.h"
#include "StrData.h"
#include <fstream>
using namespace std;

VarString::VarString(Feeder *f, Environment *env) : Element(f,env)
{
	m_condition = NULL;
	m_data = NULL;
}

VarString::~VarString()
{
}

bool VarString::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	bool res = m_feeder->str("str") 
			&& m_feeder->variable(&m_var_name) 
			&& m_feeder->str("=");

	if(!res){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	string tmpdir = m_env->getImportPaths("tmpdir")->at(0);

	m_feeder->getPos(&m_end_line, &m_end_char);

	try{
		m_data = new StrData();
		string f = m_env->m_tmpdir + "/" + m_var_name;
		m_data->setFifoName(&f);
		m_data->createFifo();
		m_env->setData(&m_var_name,m_data);
	}catch(Environment *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 1;
		throw this;
	}catch(StrData *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 1;
		throw this;
	}

	return true;
}

// joint the redirect
int VarString::exec(void)
{
	m_data->openFifo();
	return 0;
}

/*
 * This function reads a named pipe and append the output to
 * the value of this variable.
 * This procedure is not optimized. Very slow.
 * However, It's not critical because this string
 * should not contain a long string.
 */
bool VarString::readFifo(void)
{
	try{
		m_data->readFifo(m_condition);
	}catch(StrData *e){
		m_error_msg = e->m_error_msg;
		m_exit_status = 3;
		throw this;
	}

	return true;
}
