// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "DefFile.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "Environment.h"
#include "DataFile.h"
using namespace std;

DefFile::DefFile(Feeder *f,Environment *env, vector<int> *scopes) : Element(f,env,scopes)
{
	m_data = NULL;
}

DefFile::~DefFile()
{

}

bool DefFile::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	string var_name;
	bool res = m_feeder->str("file") 
			&& m_feeder->variable(&var_name) 
			&& m_feeder->str("=");

	/*if(m_env->m_is_local){
		var_name = "local." + var_name;	
	}*/

	if(!res){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);

	try{
		m_data = new DataFile();
		string filename = m_env->m_tmpdir + "/" + var_name + "-" + to_string(m_scopes.back());
		m_data->setData(&filename);
		m_env->setData(m_scopes.back(),&var_name,m_data);
	}catch(Environment *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 3;
		throw this;
	}catch(Data *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 3;
		throw this;
	}
	return true;
}

// joint the redirect
void DefFile::connect(void)
{
	try{
		m_data->openFile();
	}catch(DataFile *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 1;
		throw this;
	}

	if(m_env->m_v_opt)
		cerr << "+ pid " << getpid() << " file "
		<< m_data->m_value << " created" << endl;
}

