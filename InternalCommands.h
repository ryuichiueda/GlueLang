#ifndef __INT_COM_H_
#define __INT_COM_H_

class Environment;
class Feeder;
class CommandLine;

class InternalCommands{
public:
	static int exec(int argc, char const** argv,Environment *e,Feeder *f,CommandLine *p);
private:
	static int echo(int argc, char const** argv);
	static int pid(int argc, char const** argv,Environment *e);
/*
	static int gluewait(int argc, char const** argv,Environment *e,CommandLine *p);
	static int subwait(int pid,Environment *e,CommandLine *p);
*/
};

#endif
