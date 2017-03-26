// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __PIPELINE_H_
#define __PIPELINE_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;
class Arg;
class Exe;
class DefFile;
class DefStr;
class Where;

class Pipeline : public Element
{
public:
	Pipeline(Feeder *f, Environment *env,vector<int> *scopes);;
	virtual ~Pipeline();
	virtual void print(int indent_level = 0);

	virtual int exec(DefFile *f, DefFile *ef, DefStr *s);
	virtual bool parse(void);
	int execWait(void);

	bool m_has_and;
	bool m_has_then;
	bool m_has_or;
	bool m_is_then;
protected:
	void waitCommands(int pid);
	vector<int> m_pids;
};
#endif
