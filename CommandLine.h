#ifndef __COMMAND_LINE_H_
#define __COMMAND_LINE_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Arg;
class TmpFile;
class VarString;
class Environment;
class Where;

class CommandLine : public Element
{
public:
	CommandLine(Feeder *f, Environment *env);
	virtual ~CommandLine();
	virtual bool parse(void);
	void parseArgs(void);

	void setName(string s);
	void appendArg(string a);

	virtual int exec(void);
	virtual bool eval(void);

	void setPipe(int *pip,int prev);
	int getPrevPipe(void){return m_pipe_prev;};


	void setIfFlag(void){m_if = true;}
	bool getIfFlag(void){return m_if;}

	bool m_is_wait;

	const char** makeArgv(void);
protected:
	void execCommandLine(void);
	void execErrorExit(void);

	void childPipeProc(void);
	void parentPipeProc(void);


	int m_pipe[2];
	int m_pipe_prev;

	bool m_if;
};
#endif
