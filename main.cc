#include <iostream>
#include "Feeder.h"
#include "Environment.h"
#include "Script.h"
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
using namespace std;

int scriptPos(int argc, char const* argv[]);

int main(int argc, char const* argv[])
{
///////////////////////////////////////////
// reading args
//////////////////////////////////////////
	if(argc <= 1)
		exit(0);

	bool v_opt = false;
	int result = 0;
	while((result = getopt(argc,(char**)argv,"v"))!=-1){
		switch(result){
		case 'v':
			v_opt = true;
			break;
		}
	}
///////////////////////////////////////////
// initialization of top level objects 
//////////////////////////////////////////
	int script_pos = scriptPos(argc,argv);
	ifstream ifs(argv[script_pos]);
	Feeder feeder(&ifs);
	Environment env(argc,argv,script_pos);

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
		cerr << "\nParse error at " ;
		cerr << e->pos() << endl;
		e->printErrorPart();
		cerr << "\n\t" << e->m_error_msg << endl;
		cerr << "\t";

		int es = e->getExitStatus();
		cerr << '\n';
		cerr <<  "\tprocess_level " << e->getLevel() << endl;
		cerr << "\texit_status " << es << endl;
		cerr << "\tpid " << getpid() << '\n' << endl;
		env.removeFiles();

		exit(es);
	}
	catch(...){
		cerr << "\nParse error" << endl;
		cerr << "unknown error" << endl;
		env.removeFiles();
		exit(1);
	}

	if(v_opt)
		s.printOriginalString();
		
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
		cerr << "\nExecution error at " ;
		cerr << e->pos() << endl;
		e->printErrorPart();
		cerr << "\n\t" << e->m_error_msg << endl;
		env.removeFiles();
		cerr << "\t";
		//perror("ERROR: exec() failed");

		int es = e->getExitStatus();
		cerr << '\n';
		cerr <<  "\tprocess_level " << e->getLevel() << endl;
		cerr << "\texit_status " << es << endl;
		cerr << "\tpid " << getpid() << '\n' << endl;
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
