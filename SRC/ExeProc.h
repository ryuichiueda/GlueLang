#ifndef __SUB_SHELL_H_
#define __SUB_SHELL_H_

#include "Exe.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeProc : public Exe
{
public:
	ExeProc(Feeder *f, Environment *env);
	virtual ~ExeProc();
	virtual bool parse(void);
protected:
	void execChild(void);
};
#endif
