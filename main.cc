#include <iostream>
#include "Feeder.h"
#include "Command.h"
#include <fstream>
using namespace std;

int main(int argc, char const* argv[])
{
	ifstream ifs(argv[1]);
	Feeder feeder(&ifs);

	Command c(&feeder);
	c.parse();
/*
	c.setName(com);
	c.appendArg(arg);
*/
	c.exec();

	return 0;
}
