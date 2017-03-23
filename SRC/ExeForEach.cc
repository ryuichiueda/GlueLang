// Copyright 2017 Ryuichi Ueda
// Released under the MIT License.
#include "ExeForEach.h"
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

ExeForEach::ExeForEach(Feeder *f, Environment *env) : Exe(f,env)
{
}

ExeForEach::~ExeForEach()
{
}

bool ExeForEach::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	string scr;
	bool scr_exist = m_feeder->forEachBlock(&scr);
	if(!scr_exist){
		m_feeder->setPos(m_start_line, m_start_char);
		return false;
	}

	m_name = "unnamed" + to_string(m_start_line);

	//create a file
	string tmpdir = m_env->m_tmpdir + "/" + m_name;
	ofstream ofs(tmpdir.c_str());
	ofs << scr;

	auto *p = new DataProc();
	p->setFileName(&tmpdir);
	m_env->setData(&m_name,p);
	return true;
}

void ExeForEach::execChild(void)
{
	string tmpdir = m_env->m_tmpdir + "/" + m_name;

	string line;
	while (getline(cin, line)){
		vector<string> ws;
		line += ' ';//add a space at the end of a line
		int from = 0;
		for(int i=from;i<line.size();i++){
			if(line.at(i) == ' '){
				ws.push_back(line.substr(from,i-from));
				from = i + 1;
			}
		}

		int pid = fork();
		if(pid < 0)
			exit(4);

		if(pid == 0){//child process (procedures in the loop)
			auto argv = new char* [ws.size()+2];
			argv[0] = (char *)tmpdir.c_str();
			for(size_t i=0;i<ws.size();i++)
				argv[i+1] = (char *)(ws[i].c_str());
			argv[ws.size()+1] = NULL;

			vOptProc(argv[0]);
			ifstream ifs(argv[0]);
			Feeder feeder(&ifs);
		
			m_env->initExeProc((const char**)argv);
			Script s(&feeder,m_env);
		
			s.setSilent();
			s.parse();
			s.exec(); // exit in the exec function
		}else{
			int options = 0;
			int status;
			int wpid = waitpid(pid,&status,options);
			if(wpid < 1){
				m_error_msg = "Command wait error";
				m_exit_status = 2;
				throw this;
			}

			if(!WIFEXITED(status)){
				if(WIFSIGNALED(status) && WTERMSIG(status) == 13){//sigpipe
					continue;
				}else{
					exit(0);
				}
			}else{
			       	m_exit_status = WEXITSTATUS(status);
			       	if(m_exit_status == 0){
					continue;
				}else if(m_exit_status != 1){
					exit(m_exit_status);
				}else{
					exit(0);
				}
			}
		}
	}
	exit(0);
}