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

	void pushOutFile(Element *e);
protected:
	void execCommandLine(void);
	void execErrorExit(void);

	void childPipeProc(void);
	void parentPipeProc(void);

	const char** makeArgv(int file_num);

	bool m_file_to_write;

	//bool setRedirectTo(TmpFile *f);

	int m_pipe[2];
	int m_pipe_prev;
	bool m_is_piped;
};
#endif
