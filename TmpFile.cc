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
}

TmpFile::~TmpFile()
{

}

bool TmpFile::parse(void)
{
	if(! m_feeder->declare(&m_var_name,string("file")))
		return false;

	string tmpdir;
	m_env->getImportPath("tmpdir",&tmpdir);
		
	m_file_name = m_env->m_tmpdir + "/" + m_var_name;
	return true;
}

// open the file
bool TmpFile::eval(void)
{
	m_env->setVariable(&m_var_name,&m_file_name);
	m_env->setFileList(&m_file_name);

	return true;
}

// joint the redirect
int TmpFile::exec(void)
{
	m_fd = open( m_file_name.c_str() ,O_WRONLY | O_CREAT,0700);
	if(m_fd < 3){
		m_error_msg = "file: " + m_var_name + " does not open.";
		//return false;
		throw this;
	}
	if(dup2(m_fd,1) < 0){
		m_error_msg = "file: " + m_var_name + "  redirect error";
		throw this;
		//return -1;
	}
	if( close(m_fd) < 0){
		m_error_msg = "file: " + m_var_name + "  redirect error";
		throw this;
		//return -1;
	}

	return 0;
}

void TmpFile::printOriginalString(void)
{
	cerr << m_var_name;
}
