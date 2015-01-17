#include "Element.h"
#include "Feeder.h"
#include "Where.h"
#include "TmpFile.h"
#include "Procedure.h"
#include "IfBlock.h"
#include "Pipeline.h"
#include "AndLine.h"
#include "CommandLine.h"
#include "Environment.h"
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
using namespace std;

Where::Where(Feeder *f, Environment *env) : Element(f,env)
{
}

Where::~Where()
{
}

bool Where::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);
	int base_indent = m_feeder->countIndent();
	if(base_indent <= 0)
		return false;

	m_feeder->blank();
	if(!m_feeder->str("where")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	int indent = base_indent;
	while(indent >= base_indent){
		m_feeder->blank();
		if( ! add(new Pipeline(m_feeder,m_env)) &&
		! add(new Andline(m_feeder,m_env))){
			m_error_msg = "Invalid where sentences";	
			m_exit_status = 1;
			throw this;
		}

		if(!m_feeder->atNewLine()){
			m_error_msg = "Invalid pipeline or andline";	
			m_exit_status = 1;
			throw this;
		}
		
		indent = m_feeder->countIndent();
	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

int Where::exec(void)
{
	for(auto n : m_nodes)
		n->exec();

	return 0;
}

bool Where::eval(void)
{
	for(auto n : m_nodes){
		n->eval();
		cerr << ((TmpFile *)n)->m_var_name << endl;
	}
	return true;
}
