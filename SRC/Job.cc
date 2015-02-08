#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "CommandLine.h"
#include "TmpFile.h"
#include "VarString.h"
#include "Pipeline.h"
#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
#include "JobData.h"
#include "Job.h"
#include "Where.h"
using namespace std;

Job::Job(Feeder *f, Environment *env) : Element(f,env)
{
	m_if = false;
	m_outfile = NULL;
	m_outstr = NULL;
	m_where = NULL;
	m_is_background = false;
}

Job::~Job()
{
	if(m_where != NULL)
		delete m_where;
}

void Job::print(int indent_level)
{
}

/* parse of a "and line", which is a set of pipelines

	file f = command ... >>= command ... >> ...
	command ... >> command ... >>= ...

* to do:
	to implement file redirection for standard error, like
	file f1 f2 = command ... 

	The way of writting for redirection of standard error
	toward one command line will be an issue.
*/
bool Job::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	// scanning of file name 
	// If `file <filename> =` is found, 
	// the TmpFile object is pushed as the first element of m_nodes.
	// This node is also given to the last command line.
	if(add(new TmpFile(m_feeder,m_env))){
		m_outfile = (TmpFile *)m_nodes[0];	
		m_nodes.pop_back();
	}else if(add(new VarString(m_feeder,m_env))){
		m_outstr = (VarString *)m_nodes[0];	
		m_nodes.pop_back();
	}

	int comnum = 0;
	while(1){
		if(add(new Pipeline(m_feeder,m_env)))
			comnum++;

		m_feeder->getPos(&m_end_line, &m_end_char);
		m_feeder->blank();

		if(! m_feeder->str(">>")){
			m_feeder->setPos(m_end_line, m_end_char);
			break;
		}
	}

	if(comnum < 1){
		m_feeder->setPos(m_start_line,m_start_char);
		return false;
	}

	// whether background proc or not
	if(m_feeder->str("&")){
		m_is_background = true;
		if(! m_feeder->variable(&m_job_name)){
			m_error_msg = "no job name";
			m_exit_status = 1;
			throw this;
		}

		JobData *d = new JobData();
		d->setData(&m_job_name);

		try{
			m_env->setData(&m_job_name,d);
		}catch(Environment *e){
			m_error_msg = e->m_error_msg;	
			m_exit_status = 1;
			throw this;
		}
		
/*
		if(!m_env->initBG(&m_job_name)){
			m_error_msg = "Name confliction";
			m_exit_status = 1;
			throw this;
		}
*/
	}

	if(add(new Where(m_feeder,m_env))){
		m_where = (Where *)m_nodes.back();
		m_nodes.pop_back();
		// give conditions to strings
		if(m_outstr != NULL){
			m_outstr->m_condition = m_where->findCond(&m_outstr->m_var_name);
		}

	}

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

int Job::exec(void)
{
	// stdout of all commands are appended into a file
	for(auto *n : m_nodes){
		((Pipeline *)n)->m_if = m_if;
		((Pipeline *)n)->m_outfile = m_outfile;
		((Pipeline *)n)->m_outstr = m_outstr;
	}

	cout << flush;

	eval();

	if(m_where != NULL)
		m_where->exec();

	if(m_is_background)
		return execBackGround();

	return execNormal();
}

int Job::execNormal(void)
{
	for(int i=0;i<(int)m_nodes.size();i++){
		auto *p = (Pipeline *)m_nodes[i];
		if(m_outfile != NULL && i!=0)
			m_outfile->m_append_mode = true;

		int es = p->exec();
		if(m_if && es != 0)
			return es;
	}
	return 0;
}

int Job::execBackGround(void)
{
	int pid = fork();
	if(pid < 0)
		exit(1);

	if (pid == 0){//child
		if(m_env->m_v_opt)
			cerr << "+ pid " << m_env->m_pid << " fork " << m_job_name << endl;

		for(int i=0;i<(int)m_nodes.size();i++){
			auto *p = (Pipeline *)m_nodes[i];
			if(m_outfile != NULL && i!=0)
				m_outfile->m_append_mode = true;
	
			int es = p->exec();
			if(m_if && es != 0)
				exit(es);
		}
		exit(0);
	}

	try{
		JobData *p = (JobData *)m_env->getData(&m_job_name);
		p->m_pid = pid;
	}catch(...){
		m_error_msg = "Bug of backgound process";
		m_exit_status = 1;
		throw this;
	}

	return 0;
}
