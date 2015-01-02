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
}

VarString::~VarString()
{

}

bool VarString::parse(void)
{
	if(! m_feeder->declare(&m_var_name,string("str")))
		return false;

	m_file_name = "/tmp/" + to_string(getpid()) + "-" + m_var_name;
	return true;
}

// open the file
bool VarString::eval(void)
{
	m_env->setVariable(&m_var_name,&m_file_name);
	m_env->setFileList(&m_file_name);
	m_fd = open( m_file_name.c_str() ,O_WRONLY | O_CREAT,0700);
	if(m_fd < 3){
		m_error_messages.push_back("file: " + m_var_name + " does not open.");
		return false;
	}

	return true;
}

// joint the redirect
int VarString::exec(void)
{
	if(dup2(m_fd,1) < 0){
		m_error_messages.push_back("file: " + m_var_name + "  redirect error");
		return -1;
	}
	if( close(m_fd) < 0){
		m_error_messages.push_back("file: " + m_var_name + "  redirect error");
		return -1;
	}

	return 0;
}

void VarString::printOriginalString(void)
{
	cerr << m_var_name;
}
