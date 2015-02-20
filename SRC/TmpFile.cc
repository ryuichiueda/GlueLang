#include "TmpFile.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "Environment.h"
#include "FileData.h"
using namespace std;

TmpFile::TmpFile(Feeder *f, Environment *env) : Element(f,env)
{
	m_data = NULL;
}

TmpFile::~TmpFile()
{

}

bool TmpFile::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	string var_name;
	bool res = m_feeder->str("file") 
			&& m_feeder->variable(&var_name) 
			&& m_feeder->str("=");

	if(!res){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_feeder->getPos(&m_end_line, &m_end_char);

	try{
		m_data = new FileData();
		string filename = m_env->m_tmpdir + "/" + var_name;
		m_data->setData(&filename);
		m_env->setData(&var_name,m_data);
	}catch(Environment *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 1;
		throw this;
	}catch(Data *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 1;
		throw this;
	}
	return true;
}

// joint the redirect
int TmpFile::exec(void)
{
	try{
		m_data->openFile();
	}catch(FileData *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 1;
		throw this;
	}

	if(m_env->m_v_opt)
		cerr << "+ pid " << getpid() << " file "
		<< m_data->m_value << " created" << endl;

	return 0;
}

