#include "Array.h"
#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
using namespace std;

Array::Array(Feeder *f,Environment *env) : Element(f,env)
{
}

Array::~Array()
{
}

bool Array::eval(void)
{
	for(auto e : m_nodes){
		e->eval();
	}

	return true;
}

int Array::exec(void)
{
	return 0;
}

bool Array::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	while(1){
		if(! add(new Arg(m_feeder,m_env)))
			break;

		m_feeder->blank();

		if(m_feeder->atNewLine())
			break;
	}

	return true;
}
