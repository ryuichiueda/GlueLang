// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "Exe.h"
#include "DefFile.h"
#include "DefStr.h"
#include "Pipeline.h"
#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
#include "DataJob.h"
#include "DataFile.h"
#include "Job.h"
#include "Where.h"
using namespace std;

Job::Job(Feeder *f, Environment *env) : Element(f,env)
{
	m_outfile = NULL;
	m_outstr = NULL;
	m_where = NULL;
	m_is_background = false;

	m_local_env = NULL;
}

Job::~Job()
{
	if(m_where != NULL)
		delete m_where;
}

void Job::print(int indent_level)
{
}

/* parse of a job, which is a set of pipelines combined by >> or !>.

	file f = command ... >>= command ... >> ...
	command ... !> command ... >>= ...

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
	// the DefFile object is pushed as the first element of m_nodes.
	// This node is also given to the last command line.
	if(add(new DefFile(m_feeder,m_env))){
		m_outfile = (DefFile *)m_nodes[0];	
		m_nodes.pop_back();
	}else if(add(new DefStr(m_feeder,m_env))){
		m_outstr = (DefStr *)m_nodes[0];	
		m_nodes.pop_back();
	}

	int comnum = 0;
	while(1){
		if(add(new Pipeline(m_feeder,m_env)))
			comnum++;
		else
			break;

		m_feeder->getPos(&m_end_line, &m_end_char);
		m_feeder->blank();

		size_t num = m_nodes.size();
		Pipeline *back1 = (Pipeline *)m_nodes.back();
		Pipeline *back2 = num >= 2 ? (Pipeline *)m_nodes[num-2] : NULL;

		if(m_feeder->str(">>")){
			back1->m_has_and = true;
		}else if(m_feeder->str("?>")){
			back1->m_has_then = true;
		}else if(m_feeder->str("!>")){
			if(back2 != NULL and ( back2->m_has_and or back2->m_has_then) )
				back2->m_has_or = true;
			else
				back1->m_has_or = true;
		}else{
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
			m_exit_status = 2;
			throw this;
		}

		DataJob *d = new DataJob();
		d->setData(&m_job_name);

		try{
			m_env->setData(&m_job_name,d);
		}catch(Environment *e){
			m_error_msg = e->m_error_msg;	
			m_exit_status = 2;
			throw this;
		}
	}

	setJobId(m_env->publishJobId());
	if(add(new Where(m_feeder,m_env))){
		m_where = (Where *)m_nodes.back();
		m_nodes.pop_back();
		// give conditions to strings
		if(m_outstr != NULL){
			m_outstr->m_condition = m_where->findCond(&m_outstr->m_var_name);
		}

	}
	if(m_outstr != NULL)	m_outstr->setJobId(m_job_id);
	if(m_outfile != NULL)	m_outfile->setJobId(m_job_id);
	if(m_where != NULL)	m_where->setJobId(m_job_id);

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

int Job::exec(void)
{
	// stdout of all commands are appended into a file
	for(auto *n : m_nodes){
		((Pipeline *)n)->m_outfile = m_outfile;
		((Pipeline *)n)->m_outstr = m_outstr;
	}

	cout << flush;

	eval();

	if(m_where != NULL){
		m_where->exec();
		m_local_env = m_where->m_local_env;
	}


	if(m_is_background)
		return execBackGround();

	return execNormal();
}

int Job::execNormal(void)
{
	bool skip = false; // flag to skip the next command
	bool stop_next = false; // stop after then
	for(int i=0;i<(int)m_nodes.size();i++){
		if(skip){
			skip = false;
			continue;
		}

		auto *p = (Pipeline *)m_nodes[i];
		if(m_outfile != NULL && i!=0)
			m_outfile->m_data->setAppend();

		int es = p->exec();
		if(stop_next)
			return es;

		if(p->m_has_and and es != 0){
			skip = true;
		}else if(p->m_has_or and es == 0){
			skip = true;
		}
		
		if(p->m_has_then and es == 0){
			stop_next = true;
		}else if(p->m_has_then and es != 0){
			skip = true;
		}
	}
	return 0;
}

int Job::execBackGround(void)
{
	int pid = fork();
	if(pid < 0)
		exit(4);

	if (pid == 0){//child
		if(m_env->m_v_opt)
			cerr << "+ pid " << m_env->m_pid << " fork " << m_job_name << endl;

		for(int i=0;i<(int)m_nodes.size();i++){
			auto *p = (Pipeline *)m_nodes[i];
			if(m_outfile != NULL && i!=0)
				m_outfile->m_data->setAppend();
	
			p->exec();
		}
		exit(0);
	}

	try{
		DataJob *p = (DataJob *)m_env->getData(&m_job_name);
		p->m_pid = pid;
	}catch(...){
		m_error_msg = "Bug of backgound process";
		m_exit_status = 2;
		throw this;
	}

	return 0;
}
