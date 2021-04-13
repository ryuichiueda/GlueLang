// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __INT_COM_H_
#define __INT_COM_H_

#include <string>
using namespace std;

class Environment;
class Feeder;
class Executable;

class InternalCommands{
public:
	static int exec(char const** argv,Environment *e,Feeder *f,Executable *p);
	static bool exist(string *name);
private:
	static int echo(int argc, char const** argv);
	static int break_(Environment *e);
	//static int repeat(int argc, char const** argv, Environment *e);
	//static int while_(int argc, char const** argv, Environment *e);
	static int pid(int argc,Environment *e);
	static int ppid(int argc,Environment *e);
};

#endif
