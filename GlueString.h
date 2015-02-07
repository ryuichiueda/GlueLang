#ifndef __STRING_PROC_H_
#define __STRING_PROC_H_

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

class GlueString : public Element
{
public:
	GlueString(Feeder *f, Environment *env);
	virtual ~GlueString();
	virtual bool parse(void);
	virtual int exec(void);
	virtual bool eval(void);

	string m_text;

//	const char** makeArgv(void);
};
#endif
