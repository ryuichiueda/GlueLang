// Copyright 2017 Ryuichi Ueda
// Released under the MIT License.
#include "ExecWhile.h"
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

ExecWhile::ExecWhile(Feeder *f, Environment *env,vector<int> *scopes) : Executable(f,env,scopes)
{
}

ExecWhile::~ExecWhile()
{
}

bool ExecWhile::parse(void)
{
	m_feeder->getPos(&m_start_line, &m_start_char);

	string scr;
	bool scr_exist = m_feeder->whileBlock(&scr);
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
	m_env->setData(m_scopes.back(),&m_name,p);
	return true;
}

void ExecWhile::execChild(DefFile *f,  DefStr *s)
{
	auto argv = new char* [2];
	string tmpdir = m_env->m_tmpdir + "/" + m_name;
	argv[0] = (char *)tmpdir.c_str();
	argv[1] = NULL;

	while(1){
		int pid = fork();
		if(pid < 0)
			exit(4);

		if(pid == 0){//child process (procedures in the loop)
			vOptProc(argv[0]);
			ifstream ifs(argv[0]);
			Feeder feeder(&ifs);
		
			m_env->initExecProc((const char**)argv);
			Script scr(&feeder,m_env,NULL);
		
			scr.setSilent();
			scr.parse();
			scr.exec(f,s); // exit in the exec function
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
				}else if(WIFSIGNALED(status) && WTERMSIG(status) == SIGUSR1){
					exit(0);
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
}
