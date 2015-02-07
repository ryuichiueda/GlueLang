#ifndef __EXT_COMMAND_H_
#define __EXT_COMMAND_H_

#include "CommandLine.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExtCommand : public CommandLine
{
public:
	ExtCommand(Feeder *f, Environment *env);
	virtual ~ExtCommand();
	virtual bool parse(void);
protected:
	void execChild(void);
};
#endif
