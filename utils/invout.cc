#include <iostream>
#include <unistd.h>
using namespace std;


int main(int argc, char const* argv[])
{
	dup2(1, 3);
	dup2(2, 1);
	dup2(3, 2);

	cout << "stdout" << endl;
	cerr << "stderr" << endl;
		/*
	int pid = fork();

	if(pid == 0){
	}else{
		cout << "child" << endl;
	}
	*/

	exit(0);
}
