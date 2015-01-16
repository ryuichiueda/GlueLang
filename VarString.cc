#include "VarString.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "Environment.h"
using namespace std;

VarString::VarString(Feeder *f, Environment *env) : Element(f,env)
{
	m_fd = -1;
	m_opened = false;
	m_evaled = false;
	m_is_set = false;
}

VarString::~VarString()
{

}

bool VarString::parse(void)
{
	if(! m_feeder->declare(&m_var_name,string("str")))
		return false;

	string tmpdir = m_env->getImportPaths("tmpdir")->at(0);
		
	m_file_name = tmpdir + to_string(getpid()) + "-" + m_var_name;
	return true;
}

// open the file
bool VarString::eval(void)
{
	if(m_evaled)
		return true;

	if(mkfifo(m_file_name.c_str(),0700) != 0){
		m_error_msg = "str: " + m_var_name + " " 
				+ "(named pipe " + m_file_name.c_str() + ") does not prepared.";
		throw this;
	}
	m_evaled = true;
	return true;
}

// joint the redirect
int VarString::exec(void)
{
	if(m_opened)
		return true;

	m_fd = open( m_file_name.c_str() ,O_WRONLY ,0700);
	if(dup2(m_fd,1) < 0){
		m_error_msg = "str: " + m_var_name + "  redirect error";
		throw this;
		//return -1;
	}
	if( close(m_fd) < 0){
		m_error_msg = "str: " + m_var_name + "  redirect error";
		throw this;
		//return -1;
	}

	m_opened = true;
	return 0;
}

/*
 * This function reads a named pipe and append the output to
 * the value of this variable.
 * This procedure is not optimized. Very slow.
 * However, It's not critical because this string
 * should not contain a long string.
 */
bool VarString::readFiFo(void)
{
	ifstream ifs(m_file_name.c_str());
	string tmp;
	string value;
	if(m_is_set){
		value = "\n";
	}

	bool isfirst = true;
	while(ifs && getline(ifs, tmp)){
		if(isfirst){
			value += tmp;
			isfirst = false;
		}else
			value += "\n" + tmp;
	}

	if(m_is_set){
		m_env->appendValue(&m_var_name,&value);
		return true;
	}

	m_env->setVariable(&m_var_name,&value);
	m_env->setFileList(&m_file_name);
	m_is_set = true;

	return true;
}
