#include "Command.h"
#include "Feeder.h"
using namespace std;

Command::Command(Feeder *f) : Element(f)
{
}

Command::~Command()
{
}

void Command::print(int indent_level)
{
	for(int i=0;i<indent_level;i++)
		cout << '\t';

	cout << m_text << endl;
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
