#ifndef __INT_COM_H_
#define __INT_COM_H_

class Environment;
class Feeder;

class InternalCommands{
public:
	static int exec(int argc, char const** argv,Environment *e,Feeder *f);
private:
	static int echo(int argc, char const** argv);
	static int pid(int argc, char const** argv,Environment *e);
};

#endif
