#include <iostream>
#include "Feeder.h"
#include "Script.h"
#include <fstream>
using namespace std;

int main(int argc, char const* argv[])
{
	ifstream ifs(argv[1]);
	Feeder feeder(&ifs);

	Script script(&feeder);


/*
	// read the script
	vector<char> script((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());

	//do
	Script s(&script,0);

	if(!s.isSet()){
		cerr << "glue: script parse error" << endl;
	}
*/
	return 0;
}
