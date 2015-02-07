#ifndef __INT_COMMAND_H_
#define __INT_COMMAND_H_

#include "CommandLine.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class IntCommand : public CommandLine
{
public:
	IntCommand(Feeder *f, Environment *env);
	virtual ~IntCommand();
	virtual bool parse(void);
protected:
	void execChild(void);
};
#endif
