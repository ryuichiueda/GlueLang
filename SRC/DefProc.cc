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

DefProc::DefProc(Feeder *f, Environment *env) : Element(f,env)
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

	if(!m_feeder->smallCaps(&m_name)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}
	m_feeder->blank();

	if(!m_feeder->str("=")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	string tmp;
	if(!m_feeder->lineResidual(&tmp)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	// reading first line
	int i = 0;
	for(;i< (int)tmp.size();i++){
		if(tmp[i] != ' ' && tmp[i] != '\t')
			break;
	}
	int indent = i;

	//create a file
	string tmpdir = m_env->m_tmpdir + "/" + m_name;
	ofstream ofs(tmpdir.c_str());
	ofs << tmp.substr(indent,tmp.size()-indent) << endl;

	auto *p = new DataProc();
	p->setFileName(&tmpdir);
	m_env->setData(&m_name,p);
	
	while(m_feeder->blankLine()){
		ofs << endl;
	}

	//read other lines
	// The second line fixes the offside line of this procedure
	indent = m_feeder->countIndent();
	int idt = indent;
	// no indent -> the while loop should be avoided
	if(idt == 0)
		idt = -1;

	while(idt >= indent){//write the script file with removal of the indent
		m_feeder->lineResidual(&tmp);
		ofs << tmp.substr(indent,tmp.size()-indent) << endl;

		while(m_feeder->blankLine()){
			ofs << endl;
		}
		idt = m_feeder->countIndent();
	}

	ofs.close();
	m_feeder->getPos(&m_end_line, &m_end_char);
	//m_script.push_back(tmp);
	return true;
}
