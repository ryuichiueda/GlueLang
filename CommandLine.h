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

class CommandLine : public Element
{
public:
	CommandLine(Feeder *f, Environment *env);
	virtual ~CommandLine();
	bool parse(void);

	void setName(string s);
	void appendArg(string a);

	virtual int exec(void);
	virtual bool eval(void);

	virtual void printOriginalString(void);

	void setPipe(int *pip,int prev);
	int getPrevPipe(void){return m_pipe_prev;};

	void pushOutFile(TmpFile *e);
	void pushVarString(VarString *e);

	void setIfFlag(void){m_if = true;}
protected:
	void execCommandLine(void);
	void execErrorExit(void);

	void childPipeProc(void);
	void parentPipeProc(void);

	const char** makeArgv(int file_num);

	TmpFile *m_outfile;
	VarString *m_outstr;

	int m_pipe[2];
	int m_pipe_prev;
	bool m_is_piped;

	bool m_if;
};
#endif
