#include "Command.h"
#include "Feeder.h"
#include "Environment.h"
using namespace std;

Command::Command(Feeder *f, Environment *env) : Element(f,env)
{
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
	return m_feeder->command(&m_text);
}

const char *Command::getStr(void)
{
	return m_text.c_str();
}

void Command::printOriginalString(void)
{
	cerr << m_text << " ";
}
