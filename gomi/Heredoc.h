#ifndef __HEREDOC_H_
#define __HEREDOC_H_

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

class Heredoc : public Element
{
public:
	Heredoc(Feeder *f, Environment *env);
	virtual ~Heredoc();
	virtual void print(int indent_level = 0);

	virtual int exec(void);
	virtual bool eval(void);
	virtual bool parse(void);
private:
	int m_indent;
};
#endif
