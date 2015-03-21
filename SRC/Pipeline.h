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
	Pipeline(Feeder *f, Environment *env);
	virtual ~Pipeline();
	virtual void print(int indent_level = 0);

	virtual int exec(void);
	virtual bool parse(void);
	int execWait(void);

	DefFile *m_outfile;
	DefStr *m_outstr;

	bool m_if;
	bool m_rev_connect;
protected:
	void waitCommands(int pid);
	vector<int> m_pids;
};
#endif