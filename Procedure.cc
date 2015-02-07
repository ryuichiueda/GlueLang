#include "Procedure.h"
#include "Command.h"
#include "CommandLine.h"
#include "Feeder.h"
#include "Environment.h"
#include <fstream>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
using namespace std;

Procedure::Procedure(Feeder *f, Environment *env) : Element(f,env)
{
}

Procedure::~Procedure()
{
}

bool Procedure::eval(void)
{
	return true;
}

int Procedure::exec(void)
{
	return 0;
}

bool Procedure::parse(void)
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
	string tmpdir = m_env->getImportPaths("tmpdir")->at(0);
	int pid = getpid();
	tmpdir += "glue" + to_string(pid) + "/" + m_name;

	ofstream ofs(tmpdir.c_str());
	ofs << tmp.substr(indent,tmp.size()-indent) << endl;

	m_env->setFileList(&tmpdir);

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
	m_script.push_back(tmp);
	return true;
}

