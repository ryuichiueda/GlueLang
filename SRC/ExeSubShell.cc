#include "ExeSubShell.h"
//#include "ArgCommand.h"
#include "InternalCommands.h"
#include "Environment.h"
#include "Script.h"
#include "Arg.h"
#include "Where.h"
#include "DefFile.h"
#include "DefStr.h"
#include "ArgProc.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Feeder.h"
#include "DataProc.h"
using namespace std;

ExeSubShell::ExeSubShell(Feeder *f, Environment *env) : Exe(f,env)
{
}

ExeSubShell::~ExeSubShell()
{
}

bool ExeSubShell::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	if(!m_feeder->str("(")){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_name = "unnamed" + to_string(m_start_line);

	//create a file
	string tmpdir = m_env->m_tmpdir + "/" + m_name;
	ofstream ofs(tmpdir.c_str());

	auto *p = new DataProc();
	p->setFileName(&tmpdir);
	m_env->setData(&m_name,p);
	
	while(m_feeder->blankLine()){
		ofs << endl;
	}

	//read other lines
	// The second line fixes the offside line of this procedure
	int indent = m_feeder->countIndent();
	int idt = indent;

	string tmp;
	while(! m_feeder->str(")")){
		if(idt < indent){
			m_error_msg = "Invalid indent";	
			m_exit_status = 1;
			throw this;
		}
		m_feeder->lineResidual(&tmp);
		ofs << tmp.substr(indent,tmp.size()-indent) << endl;
		//ofs << tmp << endl;

		while(m_feeder->blankLine()){
			ofs << endl;
		}
		//idt = m_feeder->countIndent();
	}

	ofs.close();
	m_feeder->getPos(&m_end_line, &m_end_char);
	//m_script.push_back(tmp);
	return true;
}

void ExeSubShell::execChild(void)
{
	auto argv = new char* [2];
	string tmpdir = m_env->m_tmpdir + "/" + m_name;
	argv[0] = (char *)tmpdir.c_str();
	argv[1] = NULL;

	vOptProc(argv[0]);
	ifstream ifs(argv[0]);
	Feeder feeder(&ifs);

	m_env->initExeProc((const char**)argv);
	Script s(&feeder,m_env);

	s.setSilent();
	s.parse();
	s.exec(); // exit in the exec function
}
