#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
using namespace std;

Arg::Arg(Feeder *f,Environment *env) : Element(f,env)
{
	m_is_variable = false;
//	m_text = *str;
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
	if(m_is_variable){
		return m_env->getVariable(&m_text,&m_evaled_text);
	}

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

bool Arg::parse(void)
{
	if(m_feeder->literalString(&m_text)){
		m_is_variable = false;
		return true;
	}

	if(m_feeder->variable(&m_text)){
		m_is_variable = true;
		return true;
	}

	return false;
}

void Arg::printOriginalString(void)
{
	cerr << m_text << " ";
}
