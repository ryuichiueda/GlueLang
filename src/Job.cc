// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "Executable.h"
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

Job::Job(Feeder *f, Environment *env,vector<int> *scopes) : Element(f,env,scopes)
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
*/
bool Job::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	// scanning of file name 
	// If `file <filename> =` is found, 
	// the DefFile object is pushed as the first element of m_nodes.
	// This node is also given to the last command line.
	if(add(new DefFile(m_feeder,m_env,&m_scopes))){
		m_outfile = (DefFile *)m_nodes[0];	
		m_nodes.pop_back();
	}else if(add(new DefStr(m_feeder,m_env,&m_scopes))){
		m_outstr = (DefStr *)m_nodes[0];	
		m_nodes.pop_back();
	}

	setJobId(m_env->publishJobId());
	int comnum = 0;
	while(1){
		if(add(new Pipeline(m_feeder,m_env,&m_scopes)))
			comnum++;
		else
			break;

		m_feeder->getPos(&m_end_line, &m_end_char);
		m_feeder->blank();

		size_t num = m_nodes.size();
		Pipeline *back1 = (Pipeline *)m_nodes.back();
		Pipeline *back2 = num >= 2 ? (Pipeline *)m_nodes[num-2] : NULL;

		back1->m_is_then = back2 != NULL and back2->m_has_then;

		if(m_feeder->str(">>")){
			back1->m_has_and = true;
			if(back2 != NULL and back2->m_has_then){
				m_feeder->getPos(&m_end_line, &m_end_char);
				m_error_msg = "Invalid connection (>> is connected after ?>)";
				m_exit_status = 6;
				throw this;
			}
		}else if(m_feeder->str("?>")){
			back1->m_has_then = true;
		}else if(m_feeder->str("!>")){
			if(back2 != NULL and back2->m_has_then)
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
			m_env->setData(0,&m_job_name,d);
		}catch(Environment *e){
			m_error_msg = e->m_error_msg;	
			m_exit_status = 2;
			throw this;
		}
	}

	if(add(new Where(m_feeder,m_env,&m_scopes))){
		m_where = (Where *)m_nodes.back();
		m_nodes.pop_back();

	}
	if(m_outstr != NULL)	m_outstr->setJobId(m_job_id);
	if(m_outfile != NULL)	m_outfile->setJobId(m_job_id);
	if(m_where != NULL)	m_where->setJobId(m_job_id);

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

int Job::exec(DefFile *f,  DefStr *s)
{
	if(m_outfile != NULL)
		f = m_outfile;
	if(m_outstr != NULL)
		s = m_outstr;

	cout << flush;

	eval();

	if(m_where != NULL){
		m_where->exec(f,s);
		m_local_env = m_where->m_local_env;
	}


	if(m_is_background)
		return execBackGround(f,s);

	return execNormal(f,s);
}

int Job::execNormal(DefFile *f,  DefStr *s)
{
	bool skip = false; // flag to skip the next command
	bool stop_next = false; // stop after then
	for(int i=0;i<(int)m_nodes.size();i++){
		if(skip and not stop_next){
			skip = false;
			continue;
		}

		if(f != NULL && i!=0)
			f->m_data->setAppend();

		auto *p = (Pipeline *)m_nodes[i];
		int es = p->exec(f,s);//Sometimes it does not come back.
		if(stop_next){
			return es;
		}

		stop_next = p->m_has_then and es == 0;
		skip = (p->m_has_then and es != 0)
			or (p->m_has_and and es != 0)
			or (p->m_has_or and es == 0);
	}
	return 0;
}

int Job::execBackGround(DefFile *f,  DefStr *s)
{
	int pid = fork();
	if(pid < 0)
		exit(4);

	if (pid == 0){//child
		if(m_env->m_v_opt)
			cerr << "+ pid " << m_env->m_pid << " fork " << m_job_name << endl;

		for(int i=0;i<(int)m_nodes.size();i++){
			auto *p = (Pipeline *)m_nodes[i];
			if(f != NULL && i!=0)
				f->m_data->setAppend();
	
			p->exec(f,s);
		}
		exit(0);
	}

	try{
		DataJob *p = (DataJob *)m_env->getData(&m_scopes,&m_job_name);
		p->m_pid = pid;
	}catch(...){
		m_error_msg = "Bug of backgound process";
		m_exit_status = 2;
		throw this;
	}

	return 0;
}
