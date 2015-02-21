#ifndef __COMMAND_LINE_H_
#define __COMMAND_LINE_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Arg;
class DefFile;
class DefStr;
class Environment;
class Where;

class Exe : public Element
{
public:
	Exe(Feeder *f, Environment *env);
	virtual ~Exe();
	virtual bool parse(void) = 0;
	void parseArgs(void);

	virtual void appendArg(string a){m_add_args.push_back(a);}
	virtual void clearAppendArg(void){m_add_args.clear();}

	virtual int exec(void);
	virtual bool eval(void);

	void setPipe(int *pip,int prev);
	int getPrevPipe(void){return m_pipe_prev;};

	void setIfFlag(void){m_if = true;}
	bool getIfFlag(void){return m_if;}

	bool m_is_wait;

	char** makeArgv(void);
protected:
	virtual void execChild(void) = 0;
	void execErrorExit(void);

	void childPipeProc(void);
	void parentPipeProc(void);

	void prepareFile(void);

	int m_pipe[2];
	int m_pipe_prev;

	bool m_if;

	void vOptProc(char const *arg);

	vector<string> m_add_args;
};
#endif
