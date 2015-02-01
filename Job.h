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
class CommandLine;
class TmpFile;
class VarString;

class Job : public Element
{
public:
	Job(Feeder *f, Environment *env);
	virtual ~Job();
	virtual void print(int indent_level = 0);

	virtual int exec(void);
	virtual bool eval(void);
	virtual bool parse(void);

	bool m_if;
protected:
	int execNormal(void);
	int execBackGround(void);

	TmpFile *m_outfile;
	VarString *m_outstr;

	Where *m_where;

	string m_job_name;
	bool m_is_background;
};
#endif