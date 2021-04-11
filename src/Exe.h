// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
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
	Exe(Feeder *f,Environment *env, vector<int> *scopes);
	virtual ~Exe();
	virtual bool parse(void) = 0;
	void parseArgs(void);

	virtual void appendArg(string a){m_add_args.push_back(a);}
	virtual void clearAppendArg(void){m_add_args.clear();}

	virtual int exec(DefFile *f, DefStr *s);
	virtual bool eval(void);

	void setPipe(int *pip,int prev);
	int getPrevPipe(void){return m_pipe_prev;};

	bool m_is_wait;
	bool m_is_break;

	char** makeArgv(void);
protected:
	virtual void execChild(DefFile *f, DefStr *s) = 0;
	void execErrorExit(void);

	void childPipeProc(void);
	void parentPipeProc(void);

	void prepareFile(void);

	int m_pipe[2];
	int m_pipe_prev;

	void vOptProc(char const *arg);

	vector<string> m_add_args;
};
#endif
