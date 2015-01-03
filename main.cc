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

	ifstream ifs(argv[argc-1]);
	Feeder feeder(&ifs);
	Environment env;
	// set tmpdir
	string tmp_k = "tmpdir";
	string tmp_v = "/tmp/";
	env.setImportPath(&tmp_k,&tmp_v);

	Script s(&feeder,&env);
	try{
		s.parse();
		//s.errorCheck();
	}
	catch(Environment *e){
		cerr << e->m_error_msg << endl;
		env.removeFiles();
		exit(1);
	}
	// exit if s.parse() returns one or some errors

	if(v_opt)
		s.printOriginalString();
		
	try{
		int status = s.exec();
		env.removeFiles();

		if(status != 0)
			exit(status);

		if(feeder.atEnd())
			exit(0);
	}
	catch(Element *e){
		cerr << e->m_error_msg << endl;
		env.removeFiles();
		exit(1);
	}
	catch(Environment *e){
		cerr << e->m_error_msg << endl;
		env.removeFiles();
		exit(1);
	}

	exit(1);
}
