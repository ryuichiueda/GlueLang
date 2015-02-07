#include <iostream>
#include "Feeder.h"
#include "Environment.h"
#include "Script.h"
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
using namespace std;

int scriptPos(int argc, char const* argv[]);
void parseErrorMsg(Element *e);
void execErrorMsg(Element *e);
bool setFlags(int argc, char const* argv[],Environment *e);

static void sig_int(int sig)
{
	Element::m_signal = sig;
}

int main(int argc, char const* argv[])
{
	if(setpgid(0,0) != 0)
		exit(1);

	if(argc <= 1)
		exit(0);

///////////////////////////////////////////
// set signals
//////////////////////////////////////////
	struct sigaction sig;
	memset(&sig,'\0',sizeof(sig));

	sig.sa_handler = sig_int;
	sigaction(SIGINT,&sig,NULL);

///////////////////////////////////////////
// initialization of top level objects 
//////////////////////////////////////////
	int script_pos = scriptPos(argc,argv);

	ifstream ifs(argv[script_pos]);
	Feeder feeder(&ifs);
	Environment env(argc,argv,script_pos);

	if(!setFlags(argc,argv,&env))
		exit(1);//flags are invalid.

	// set tmpdir
	string tmp_k = "tmpdir";
	string tmp_v = "/tmp/";
	env.setImportPath(&tmp_k,&tmp_v);
	Script s(&feeder,&env);

///////////////////////////////////////////
// parse
//////////////////////////////////////////
	try{
		s.parse();
	}
	catch(Element *e){
		parseErrorMsg(e);
		env.removeFiles();
		exit(e->getExitStatus());
	}
	catch(...){
		cerr << "\nParse error" << endl;
		cerr << "unknown error" << endl;
		env.removeFiles();
		exit(1);
	}

///////////////////////////////////////////
// exec
//////////////////////////////////////////
	try{
		int status = s.exec();
		env.removeFiles();

		if(status == 0)
			exit(0);
	}
	catch(Element *e){
		execErrorMsg(e);
		env.removeFiles();
		int es = e->getExitStatus();
		if(es == 127)
			_exit(es);
		else
			exit(es);
	}
	catch(...){
		cerr << "\nExecution error" << endl;
		cerr << "unknown error" << endl;
		env.removeFiles();
		exit(1);
	}

	cerr << "unknown error (uncatched)" << endl;
	env.removeFiles();
	exit(1);
}

int scriptPos(int argc, char const* argv[])
{
	string com = string(argv[0]);
	bool shebang = com.substr(com.find_last_of('/') + 1) != "glue";

	if(shebang)
		return 0;

	int i=1;
	for(;i<argc;i++){
		if(argv[i][0] != '-')
			break;
	}
	return i;
}

void parseErrorMsg(Element *e)
{
	cerr << "\nParse error at " ;
	cerr << e->pos() << endl;
	e->printErrorPart();
	cerr << "\n\t" << e->m_error_msg << endl;
	cerr << "\t";

	cerr << '\n';
	cerr <<  "\tprocess_level " << e->getLevel() << endl;
	cerr << "\texit_status " << e->getExitStatus() << endl;
	cerr << "\tpid " << getpid() << '\n' << endl;
}

void execErrorMsg(Element *e)
{
	cerr << "\nExecution error at " ;
	cerr << e->pos() << endl;
	e->printErrorPart();
	cerr << "\n\t" << e->m_error_msg << endl;
	cerr << "\t\n";
	cerr <<  "\tprocess_level " << e->getLevel() << endl;
	cerr << "\texit_status " << e->getExitStatus() << endl;
	cerr << "\tpid " << getpid() << '\n' << endl;
}

void usage(void)
{
	cerr << "GlueLang (master branch)" << endl;
	cerr << "Usage: glue [OPTION] [FILE]" << endl;
	cerr << endl;
	cerr << "Copyright (C) 2015 Ryuichi Ueda" << endl;
	cerr << "GlueLang repo. <https://github.com/ryuichiueda/GlueLang>" << endl;
}

bool setFlags(int argc, char const* argv[],Environment *e)
{
	struct option long_opts[] = {
		{"help",0,NULL,'h'},
		{"usage",0,NULL,'h'},
		{"verbose",0,NULL,'v'},
		{0,0,0,0}
	};

	int opt = 0;
	int idx = 0;
	while((opt = getopt_long(argc,(char *const *)argv,"hv",long_opts,&idx)) != -1){
		switch (opt){
		case 'h':
			usage();
			return false;
		case 'v':
			e->m_v_opt = true;
			break;
		}
	}
	return true;
}
