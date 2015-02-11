#include "ArgExtCommand.h"
#include "Feeder.h"
#include "Environment.h"
#include <sys/stat.h>
using namespace std;

ArgExtCommand::ArgExtCommand(Feeder *f,Environment *env) : Arg(f,env)
{
}

ArgExtCommand::~ArgExtCommand()
{
}

bool ArgExtCommand::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	//If the name of a command starts from a small capital letter,
	//it should be parsed as a full-path command.
	// (this rule should be relaxed since a command can start from
	// a non small captal letter)
	
	// fullpath command
	if( ! m_feeder->smallCapsWithNum(&m_prefix)){
		m_feeder->getPos(&m_end_line, &m_end_char);
		bool res = m_feeder->command(&m_text);

		if(res == true)
			m_evaled_text = m_text;

		return res;
	}

	// chech whether m_prefix is a prefix or a command name
 	if(!m_feeder->str(".")){
		// if m_prefix is not a prefix, it is a name of a command or a procedure
		m_text = m_prefix;
		m_prefix = "";
		//m_feeder->getPos(&m_end_line, &m_end_char);
		//return true;
		return parsePrefixedCom();
	}
	//hereafter, a command name with a prefix

 	if(! m_env->isImportPath(&m_prefix)){
		m_error_msg = "Invalid path";
		m_exit_status = 1;
		m_feeder->getPos(&m_end_line, &m_end_char);
		throw this;
	}

	if(!m_feeder->command(&m_text)){
		m_error_msg = "Invalid command name";
		m_exit_status = 1;
		m_feeder->getPos(&m_end_line, &m_end_char);
		throw this;
	}

	return parsePrefixedCom();
}

/*
bool ArgExtCommand::eval(void)
{
	return true;
}
*/

bool ArgExtCommand::parsePrefixedCom(void)
{
	struct stat buf;
	auto paths = m_env->getImportPaths(&m_prefix);
	if(paths == NULL)
		return false;

	for(auto path : *paths){
		m_evaled_text = path + m_text;
		if(stat(m_evaled_text.c_str(), &buf) != 0)
			continue;

		m_feeder->getPos(&m_end_line, &m_end_char);
		return true;
	}
	m_error_msg = "Command " + m_text  + " not exist";
	m_exit_status = 1;
	m_feeder->getPos(&m_end_line, &m_end_char);
	throw this;
}
