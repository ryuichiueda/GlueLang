#include "Command.h"
#include "Feeder.h"
#include "Environment.h"
using namespace std;

Command::Command(Feeder *f, Environment *env) : Element(f,env)
{
	m_path = "";
	m_prefix = "";
}

Command::~Command()
{
}

bool Command::eval(void)
{
	return true;
}

int Command::exec(void)
{
	return 0;
}

bool Command::parse(void)
{
	int prev_ln,prev_ch;
	m_feeder->getPos(&prev_ln, &prev_ch);

	//If the name of a command starts from a small capital letter,
	//it should be parsed as a full-path command.
	// (this rule should be relaxed since a command can start from
	// a non small captal letter)
	if( ! m_feeder->smallCaps(&m_prefix))
		return m_feeder->command(&m_name);

	// chech whether m_prefix is a prefix or a command name
 	if( m_feeder->str(".") ){
 		if(m_env->isImportPath(&m_prefix)){
			m_env->getImportPath(&m_prefix, &m_path);
			return m_feeder->command(&m_name);
		}
	}

	// if m_prefix is not a prefix, it is discarded
	m_prefix = "";
	m_feeder->setPos(prev_ln,prev_ch);
	return m_feeder->command(&m_name);

}

const char *Command::getStr(void)
{
	return (m_path + m_name).c_str();	
}

void Command::printOriginalString(void)
{
	if(m_prefix != "")
		cerr << m_prefix << "." << m_name << " ";
	else
		cerr << m_name << " ";
}
