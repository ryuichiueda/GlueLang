#include "VarString.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "Environment.h"
#include "Condition.h"
using namespace std;

VarString::VarString(Feeder *f, Environment *env) : Element(f,env)
{
	m_fd = -1;
	m_opened = false;
	m_evaled = false;
	m_is_set = false;

	m_condition = NULL;
}

VarString::~VarString()
{

}

bool VarString::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(! m_feeder->declare(&m_var_name,string("str")))
		return false;

	string tmpdir = m_env->getImportPaths("tmpdir")->at(0);
	m_file_name = m_env->m_tmpdir + "/" + m_var_name;

	m_feeder->getPos(&m_end_line, &m_end_char);
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

	try{
		m_env->setFileList(&m_file_name);
	}catch(Environment *e){
		m_error_msg = e->m_error_msg;	
		m_exit_status = 1;
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

	m_fd = open( m_file_name.c_str() ,O_WRONLY ,0600);
	if(m_env->m_v_opt)
		cerr << "+ pid " << getpid() << " file " << m_file_name << " open" << endl;
	if(dup2(m_fd,1) < 0){
		m_error_msg = "str: " + m_var_name + "  redirect error";
		throw this;
	}
	if( close(m_fd) < 0){
		m_error_msg = "str: " + m_var_name + "  redirect error";
		throw this;
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

		if(m_condition != NULL){
			if(m_condition->m_upper_byte <= value.size()){
				m_error_msg = "Over length";
				m_exit_status = 3;
				throw this;
			}
		}
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
