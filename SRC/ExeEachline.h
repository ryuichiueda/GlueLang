#ifndef __EACH_LINE_H_
#define __EACH_LINE_H_

#include "Exe.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeEachline : public Exe
{
public:
	ExeEachline(Feeder *f, Environment *env);
	virtual ~ExeEachline();
	virtual bool parse(void);
protected:
	void execChild(void);
	void waitCommands(int pid);
};
#endif
