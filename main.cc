#include <iostream>
#include "Feeder.h"
#include "Environment.h"
#include "Script.h"
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
using namespace std;

int main(int argc, char const* argv[])
{
///////////////////////////////////////////
// reading args
//////////////////////////////////////////
	if(argc == 1)
		exit(1);

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
	ifstream ifs(argv[argc-1]);
	Feeder feeder(&ifs);
	Environment env;
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
	catch(Environment *e){
		cerr << e->m_error_msg << endl;
		env.removeFiles();
		exit(1);
	}
	catch(Element *e){
		cerr << e->pos() << endl;
		cerr << e->m_error_msg << endl;
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

		if(status != 0)
			exit(status);

		if(feeder.atEnd())
			exit(0);
	}
	catch(Element *e){
		cerr << "\nExecution error at " ;
		cerr << e->pos() << endl;
		e->printErrorPart();
		cerr << "\n\t" << e->m_error_msg << endl;
		env.removeFiles();
		cerr << "\t";
		perror("ERROR: exec() failed");

		int es = e->getExitStatus();
		cerr <<  "\tprocess_level " << e->getLevel() << endl;
		cerr << "\texit_status " << es << endl;
		cerr << "\tpid " << getpid() << '\n' << endl;
		if(es == 127)
			_exit(es);
		else
			exit(es);
	}
	catch(Environment *e){
		cerr << e->m_error_msg << endl;
		env.removeFiles();
		exit(1);
	}

	exit(1);
}
