#include "Element.h"
#include "Feeder.h"
#include "Where.h"
#include "DefFile.h"
#include "IfBlock.h"
#include "Pipeline.h"
#include "Exe.h"
#include "Environment.h"
#include "DefCond.h"
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <vector>
#include "Job.h"
using namespace std;

Where::Where(Feeder *f, Environment *env) : Element(f,env)
{
	m_local_env = NULL;
}

Where::~Where()
{
	if(m_local_env != NULL){
		delete m_local_env;
	}
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

	m_local_env = new Environment(m_env);

	int indent = base_indent;
	while(indent >= base_indent){
		m_feeder->blank();
		if(add(new DefCond(m_feeder,m_local_env))){
			m_conditions.push_back((DefCond *)m_nodes.back());
			m_nodes.pop_back();
		}else if(add(new Job(m_feeder,m_local_env))){
		}else{
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
	m_env->m_local_env[m_job_id] = m_local_env;
	for(auto n : m_nodes)
		n->exec();

	return 0;
}

bool Where::eval(void)
{
	for(auto n : m_nodes)
		n->eval();

	return true;
}

DefCond* Where::findCond(string *var)
{
	for(auto c : m_conditions){
		if(c->m_name == *var)
			return c;	
	}
	return NULL;
}
