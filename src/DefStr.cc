// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "DefStr.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "Environment.h"
#include "DataStr.h"
#include <fstream>
using namespace std;

DefStr::DefStr(Feeder *f, Environment *env, vector<int> *scopes)
	: Element(f,env,scopes)
{
	m_data = NULL;
}

DefStr::~DefStr()
{
}

bool DefStr::parse(void)
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
		m_data = new DataStr();
		string f = m_env->m_tmpdir + "/" + m_var_name + "-" + to_string(m_scopes.back());
		m_data->setFifoName(&f);
		m_data->createFifo();
		m_env->setData(m_scopes.back(),&m_var_name,m_data);
	}catch(Environment *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 3;
		throw this;
	}catch(DataStr *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 3;
		throw this;
	}

	return true;
}

// joint the redirect
void DefStr::connect(void)
{
	m_data->openFifo();
}

/*
 * This function reads a named pipe and append the output to
 * the value of this variable.
 * This procedure is not optimized. Very slow.
 * However, It's not critical because this string
 * should not contain a long string.
 */
bool DefStr::readFifo(void)
{
	try{
		m_data->readFifo();
	}catch(DataStr *e){
		m_error_msg = e->m_error_msg;
		m_exit_status = 3;
		throw this;
	}

	return true;
}
