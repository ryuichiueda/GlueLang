#include <iostream>
#include "Feeder.h"
#include "Command.h"
#include <fstream>
using namespace std;

int main(int argc, char const* argv[])
{
	ifstream ifs(argv[1]);
	Feeder feeder(&ifs);

	// We want to implement Parsec like LL parser by ourselves
	
	Command c(&feeder);

	if( ! c.parse() ){// attempt to parse as a command
		exit(1);
	}
		
	int status = c.exec();

	exit(status);
}
