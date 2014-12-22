#include <iostream>
#include "Feeder.h"
#include "Script.h"
#include "Command.h"
#include <fstream>
using namespace std;

int main(int argc, char const* argv[])
{
	ifstream ifs(argv[1]);
	Feeder feeder(&ifs);

	// We want to implement Parsec like LL parser by ourselves
	
	Script s(&feeder);
	
	if( ! s.parse() ){// attempt to parse as a command
		exit(1);
	}
		
	int status = s.exec();
	if(status != 0)
		exit(status);

	if(feeder.atEnd())
		exit(0);

	exit(1);
}
