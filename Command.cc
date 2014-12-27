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
	m_feeder->getCurPos(&prev_ln, &prev_ch);

	if( ! m_feeder->smallCaps(&m_prefix))
		return m_feeder->command(&m_name);

 	if(! m_feeder->str(".") || ! m_env->getImportPaths(&m_prefix, &m_path)){
		m_feeder->rewind(prev_ln,prev_ch);
	}

	return m_feeder->command(&m_name);

}

const char *Command::getStr(void)
{
	if(m_path == "")
		m_full_path = m_name;
	else
		m_full_path = m_path + m_name;

	return m_full_path.c_str();	
}

void Command::printOriginalString(void)
{
	if(m_prefix != "")
		cerr << m_prefix << "." << m_name << " ";
	else
		cerr << m_name << " ";
}
