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
	if(mkfifo(m_file_name.c_str(),0700) != 0){
		m_error_messages.push_back("str: " + m_var_name + " " 
				+ "(named pipe " + m_file_name.c_str() + ") does not prepared.");
		return false;
	}

/*
	if(m_fd < 3){
		m_error_messages.push_back("str: " + m_var_name + " " 
				+ "(named pipe " + m_file_name.c_str() + ") does not prepared.");
		return false;
	}
*/

	return true;
}

// joint the redirect
int VarString::exec(void)
{
	m_fd = open( m_file_name.c_str() ,O_WRONLY ,0700);
	if(dup2(m_fd,1) < 0){
		m_error_messages.push_back("str: " + m_var_name + "  redirect error");
		return -1;
	}
	if( close(m_fd) < 0){
		m_error_messages.push_back("str: " + m_var_name + "  redirect error");
		return -1;
	}

	return 0;
}

void VarString::printOriginalString(void)
{
	cerr << m_var_name;
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
	bool isfirst = true;
	while(ifs && getline(ifs, tmp)){
		if(isfirst){
			m_value += tmp;
			isfirst = false;
		}else
			m_value += "\n" + tmp;
	}
	m_env->setVariable(&m_var_name,&m_value);
	m_env->setFileList(&m_file_name);

	return true;
}
