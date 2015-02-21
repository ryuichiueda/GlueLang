#ifndef __EACH_LINE_H_
#define __EACH_LINE_H_

#include "Exe.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class EachLine : public Exe
{
public:
	EachLine(Feeder *f, Environment *env);
	virtual ~EachLine();
	virtual bool parse(void);
protected:
	void execChild(void);
	void waitCommands(int pid);
};
#endif
