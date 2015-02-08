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
	m_opened = false;
	m_is_set = false;

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
	m_file_name = m_env->m_tmpdir + "/" + m_var_name;

	m_feeder->getPos(&m_end_line, &m_end_char);

	try{
		m_data = new StrData();
		m_data->setFifoName(&m_file_name);
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
	if(m_opened)
		return true;

	m_data->openFifo();

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
bool VarString::readFifo(void)
{
	try{
		m_data->readFifo(m_condition,m_is_set);
	}catch(StrData *e){
		m_error_msg = e->m_error_msg;
		m_exit_status = 3;
		throw this;
	}

/*
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
			if(m_condition->m_upper_byte <= (int)value.size()){
				m_error_msg = "Over length";
				m_exit_status = 3;
				throw this;
			}
		}
	}
*/

/*
	if(m_is_set){
		m_env->appendValue(&m_var_name,&value);
		return true;
	}

	m_env->setVariable(&m_var_name,&value);
*/
	//m_env->setFileList(&m_file_name);
	m_is_set = true;
	return true;
}
