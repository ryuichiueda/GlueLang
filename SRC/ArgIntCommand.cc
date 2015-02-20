#include "ArgIntCommand.h"
#include "Feeder.h"
#include "Environment.h"
#include "InternalCommands.h"
using namespace std;

ArgIntCommand::ArgIntCommand(Feeder *f,Environment *env) : Arg(f,env)
{
}

ArgIntCommand::~ArgIntCommand()
{
}

bool ArgIntCommand::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

/*
	if(!m_feeder->str("in."))
		return false;
*/
	m_feeder->str("in.");

	string tmp;
	if(!m_feeder->variable(&m_evaled_text)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_text = "in." + m_evaled_text;

	if(m_evaled_text != "wait" && !InternalCommands::exist(&m_evaled_text)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}


	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

/*
bool ArgIntCommand::eval(void)
{
	return true;
}
*/

