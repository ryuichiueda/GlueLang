#include "Arg.h"
using namespace std;

Arg::Arg(Feeder *f) : Node(f)
{
//	parser(script,pos);
}

Arg::~Arg()
{
	cerr << "arg deleted" << endl;
}

void Arg::print(int indent_level)
{
	for(int i=0;i<indent_level;i++)
		cout << '\t';

	cout << m_text << endl;
}

void Arg::parser(vector<char> *script, int pos)
{
/*
	for(int i=pos;i<script->size();i++){
		char c = script->at(i);
		if(c == '\n')
			break;

		if(!m_set){
			if(c == ' '){
				continue;
			}else{
				m_set = true;
				m_text.append(1,c);
			}
		}else{
			if(c == ' '){
				return;
			}else{
				m_text.append(1,c);
			}
		}
	}
*/
}
