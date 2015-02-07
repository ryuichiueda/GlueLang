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

class StringProc : public Element
{
public:
	StringProc(Feeder *f, Environment *env);
	virtual ~StringProc();
	virtual bool parse(void);
	virtual int exec(void);
	virtual bool eval(void);

	string m_com;
	string m_text;

	const char** makeArgv(void);
};
#endif
