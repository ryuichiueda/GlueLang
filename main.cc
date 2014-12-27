#include <iostream>
#include "Feeder.h"
#include "Environment.h"
#include "Script.h"
#include <fstream>
#include <unistd.h>
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

	// We want to implement Parsec like LL parser by ourselves
	Script s(&feeder,&env);

	if( ! s.parse() ){
		s.printErrorMessages();
		exit(1);
	}

	if(v_opt)
		s.printOriginalString();
		
	int status = s.exec();
	env.removeFiles();

	if(status != 0)
		exit(status);

	if(feeder.atEnd())
		exit(0);

	exit(1);
}
