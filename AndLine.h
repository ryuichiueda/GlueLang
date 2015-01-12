#ifndef __ANDLINE_H_
#define __ANDLINE_H_

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

class Andline : public Element
{
public:
	Andline(Feeder *f, Environment *env);
	virtual ~Andline();
	virtual void print(int indent_level = 0);

	virtual int exec(void);
	virtual bool eval(void);
	virtual bool parse(void);

	void setIfFlag(void){m_if = true;}
	bool getIfFlag(void){return m_if;}
protected:
	TmpFile *m_outfile;
	VarString *m_outstr;

	bool m_if;
};
#endif
