#ifndef __SUB_SHELL_H_
#define __SUB_SHELL_H_

#include "CommandLine.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class SubShell : public CommandLine
{
public:
	SubShell(Feeder *f, Environment *env);
	virtual ~SubShell();
	virtual bool parse(void);
protected:
	void execChild(void);
};
#endif
