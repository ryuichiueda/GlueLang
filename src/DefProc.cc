// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "DefProc.h"
//#include "ArgCommand.h"
#include "Exe.h"
#include "Feeder.h"
#include "Environment.h"
#include "DataProc.h"
#include <fstream>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
using namespace std;

DefProc::DefProc(Feeder *f,Environment *env, vector<int> *scopes)
	: Element(f,env,scopes)
{
}

DefProc::~DefProc()
{
}

bool DefProc::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(!m_feeder->str("proc")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	if(!m_feeder->path(&m_name)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}
	m_feeder->blank();

	if(!m_feeder->str("=")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	string scr;
	bool scr_exist = m_feeder->doBlock(&scr) || m_feeder->lineResidual(&scr);
	if(!scr_exist){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	//create a file
	string tmpdir = m_env->m_tmpdir + "/" + m_name;
	ofstream ofs(tmpdir.c_str());
	ofs << scr;
	ofs.close();

	auto *p = new DataProc();
	p->setFileName(&tmpdir);
	m_env->setData(m_scopes.back(),&m_name,p);

	m_feeder->getPos(&m_end_line, &m_end_char);
	return true;
}

