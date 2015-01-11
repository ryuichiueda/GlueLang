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

	//int indent;


	if(!m_feeder->str("proc") || !m_feeder->blank(NULL)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	if(!add(new Command(m_feeder,m_env))){
		m_feeder->setPos(m_start_line, m_start_char);
		((Command *)m_nodes[0])->setPrefix("this");
		return false;
	}

	m_feeder->blank(NULL);

	if(!m_feeder->str("=")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	string tmp;
	if(!m_feeder->lineResidual(&tmp)){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	int i = 0;
	for(;i<tmp.size();i++){
		if(tmp[i] != ' ' && tmp[i] != '\t')
			break;
	}

	int indent = i;

	//create a file
	string tmpdir;
	m_env->getImportPath("tmpdir",&tmpdir);
	int pid = getpid();
	tmpdir += "glue" + to_string(pid) + "/" 
		+ *((Command *)m_nodes[0])->getName();

	ofstream ofs(tmpdir.c_str());
	ofs << tmp.substr(indent,tmp.size()-indent) << endl;

	m_env->setFileList(&tmpdir);

	ofs.close();
	m_feeder->getPos(&m_end_line, &m_end_char);
	m_script.push_back(tmp);
	return true;
}

void Procedure::printOriginalString(void)
{
	cerr << "proc ";
	m_nodes[0]->printOriginalString();
	cerr << " = " << endl;
	for(auto e : m_script){
		cerr << e << endl;
	}
	
}
