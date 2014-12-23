#include "Arg.h"
using namespace std;

Arg::Arg(string *str,Feeder *f) : Node(f)
{
	m_text = *str;
}

Arg::~Arg()
{
}

void Arg::print(int indent_level)
{
	for(int i=0;i<indent_level;i++)
		cout << '\t';

	cout << m_text << endl;
}

bool Arg::eval(void)
{
	m_evaled_text = m_text;
	//evaled escaped characters 
	auto pos = m_evaled_text.find("\\\'"); // \'
	while(pos != string::npos){
		m_evaled_text.replace(pos, 2, "\'");
		pos = m_evaled_text.find("\\\'", pos + 1);
	}
	return true;
}

int Arg::exec(void)
{
	return 0;
}
