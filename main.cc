#include <iostream>
#include "Feeder.h"
#include "Command.h"
#include <fstream>
using namespace std;

int main(int argc, char const* argv[])
{
	ifstream ifs(argv[1]);
	Feeder feeder(&ifs);

	// attempt to parse as a command
	Command c(&feeder);
	if( ! c.parse() ){
		exit(1);
	}
		
	int status = c.exec();

	exit(status);
}
