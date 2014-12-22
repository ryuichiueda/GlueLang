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

void Arg::parser(vector<char> *script, int pos)
{
}

int Arg::exec(void)
{
	return 0;
}
