#include "ArgCommand.h"
#include "Feeder.h"
#include "Environment.h"
#include <sys/stat.h>
using namespace std;

ArgCommand::ArgCommand(Feeder *f, Environment *env) : Element(f,env)
{
	m_is_proc = false;
	m_is_internal = false;
}

ArgCommand::~ArgCommand()
{
}

/*
 * /bin/echo
 * b.echo
 * echo
 * this.<procedure>
 */
bool ArgCommand::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	//If the name of a command starts from a small capital letter,
	//it should be parsed as a full-path command.
	// (this rule should be relaxed since a command can start from
	// a non small captal letter)
	
	// fullpath command
	if( ! m_feeder->smallCapsWithNum(&m_prefix)){
		m_feeder->getPos(&m_end_line, &m_end_char);
		return m_feeder->command(&m_name);
	}
	// chech whether m_prefix is a prefix or a command name
 	if(!m_feeder->str(".")){
		// if m_prefix is not a prefix, it is a name of a command or a procedure
		m_name = m_prefix;
		m_prefix = "";
		//m_feeder->getPos(&m_end_line, &m_end_char);
		//return true;
		return parsePrefixedCom();
	}
	//hereafter, a command name with a prefix

	//"this" indicetes a procedure that is defined in the script
	if(m_prefix == "this"){
		m_is_proc = true;
		m_path = m_env->m_tmpdir + "/";
		m_feeder->getPos(&m_end_line, &m_end_char);
		return m_feeder->command(&m_name);
	}else if(m_prefix == "in"){
		m_is_internal = true;
		m_path = "";
		m_feeder->getPos(&m_end_line, &m_end_char);
		return m_feeder->command(&m_name);
	}

 	if(! m_env->isImportPath(&m_prefix)){
		m_error_msg = "Invalid path";
		m_exit_status = 1;
		m_feeder->getPos(&m_end_line, &m_end_char);
		throw this;
	}

	if(!m_feeder->command(&m_name)){
		m_error_msg = "Invalid command name";
		m_exit_status = 1;
		m_feeder->getPos(&m_end_line, &m_end_char);
		throw this;
	}

	return parsePrefixedCom();
}

bool ArgCommand::parsePrefixedCom(void)
{
	struct stat buf;
	auto paths = m_env->getImportPaths(&m_prefix);
	if(paths == NULL)
		return false;

	for(auto path : *paths){
		if(stat((path + m_name).c_str(), &buf) != 0)
			continue;

		m_path = path;
		m_feeder->getPos(&m_end_line, &m_end_char);
		return true;
	}
	m_error_msg = "ArgCommand " + m_name  + " not exist";
	m_exit_status = 1;
	m_feeder->getPos(&m_end_line, &m_end_char);
	throw this;
}

const char *ArgCommand::getStr(void)
{
	m_fullpath = m_path + m_name;
	return m_fullpath.c_str();
}
