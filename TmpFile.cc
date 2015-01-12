#include "TmpFile.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "Environment.h"
using namespace std;

TmpFile::TmpFile(Feeder *f, Environment *env) : Element(f,env)
{
	m_fd = -1;
	m_evaled = false;
	m_append_mode = false;
}

TmpFile::~TmpFile()
{

}

bool TmpFile::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(!m_feeder->str("file")){
		return false;
	}

	if(! m_feeder->variable(&m_var_name)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	if(! m_feeder->str("=")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	string tmpdir;
	m_env->getImportPath("tmpdir",&tmpdir);
		
	m_file_name = m_env->m_tmpdir + "/" + m_var_name;

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

// open the file
bool TmpFile::eval(void)
{
	if(m_evaled)
		return true;

	m_evaled = true;
	try{
		m_env->setVariable(&m_var_name,&m_file_name);
		m_env->setFileList(&m_file_name);
	}catch(Environment *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 1;
		throw this;
	}
	return true;
}

// joint the redirect
int TmpFile::exec(void)
{
	int mode = O_WRONLY | O_CREAT;
	if(m_append_mode)
		mode |= O_APPEND;

	m_fd = open( m_file_name.c_str() , mode, 0700);
	if(m_fd < 3){
		m_error_msg = "file: " + m_var_name + " does not open.";
		throw this;
	}
	if(dup2(m_fd,1) < 0){
		m_error_msg = "file: " + m_var_name + "  redirect error";
		throw this;
	}
	if( close(m_fd) < 0){
		m_error_msg = "file: " + m_var_name + "  redirect error";
		throw this;
	}
	return 0;
}

void TmpFile::printOriginalString(void)
{
	cerr << m_var_name;
}
