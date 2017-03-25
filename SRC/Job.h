// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __JOB_H_
#define __JOB_H_

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

class Job : public Element
{
public:
	Job(Feeder *f, Environment *env);
	virtual ~Job();
	virtual void print(int indent_level = 0);

	virtual int exec(DefFile *f, DefFile *ef, DefStr *s, DefStr *es);
	virtual bool parse(void);

protected:
	int execNormal(DefFile *f, DefFile *ef, DefStr *s, DefStr *es);
	int execBackGround(DefFile *f, DefFile *ef, DefStr *s, DefStr *es);

	DefFile *m_outfile;
	DefStr *m_outstr;

	Where *m_where;
	Environment *m_local_env;

	string m_job_name;
	bool m_is_background;
};
#endif
