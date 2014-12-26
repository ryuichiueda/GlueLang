#include "Command.h"
#include "Feeder.h"
using namespace std;

Command::Command(Feeder *f) : Element(f)
{
//	m_text = *str;
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
/*
	/m_evaled_text = m_text;
	//evaled escaped characters 
	auto pos = m_evaled_text.find("\\\'"); // \'
	while(pos != string::npos){
		m_evaled_text.replace(pos, 2, "\'");
		pos = m_evaled_text.find("\\\'", pos + 1);
	}
*/
	return true;
}

int Command::exec(void)
{
	return 0;
}

bool Command::parse(void)
{
//bool Feeder::literalString(string *ans)

	return m_feeder->command(&m_text);
}

const char *Command::getStr(void)
{
	return m_text.c_str();
}
