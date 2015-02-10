#include "StringArray.h"
#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
#include "Literal.h"
using namespace std;

StringArray::StringArray(Feeder *f,Environment *env) : Element(f,env)
{
	m_com = "echo";
	m_connected = "";
}

StringArray::~StringArray()
{
}

bool StringArray::eval(void)
{
	for(auto e : m_nodes){
		e->eval();
	}
	return true;
}

bool StringArray::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	// solo string (no bracket)
	if(add(new Literal(m_feeder,m_env))){
		m_feeder->getPos(&m_end_line, &m_end_char);
		return true;
	}
	
	//array
	if(! m_feeder->str("[")){
		return false;
	}

	if(m_feeder->str("]")){
		m_feeder->getPos(&m_end_line, &m_end_char);
		return true;
	}

	while(add(new Literal(m_feeder,m_env))){
		if(! m_feeder->str(","))
			break;
	}

	if(m_feeder->str("]")){
		m_feeder->getPos(&m_end_line, &m_end_char);
		return true;
	}else{
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}
}

void StringArray::connect(string delimiter)
{
	bool notfirst = false;
	m_connected = "";
	for(auto *n : m_nodes){
		if(notfirst){
			m_connected += delimiter;
		}

		notfirst = true;
		m_connected += ((Literal *)n)->getEvaledString();
	}
}

const char** StringArray::makeArgv(void)
{
	// don't need to delete since the exec occurs after this procedure
	auto argv = new const char* [3];
	argv[0] = m_com.c_str();
	connect("\n");
	argv[1] = m_connected.c_str();
	argv[2] = NULL;
	return argv;
}
