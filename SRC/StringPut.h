#ifndef __STRING_PUT_H_
#define __STRING_PUT_H_

#include "CommandLine.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class StringPut : public CommandLine
{
public:
	StringPut(Feeder *f, Environment *env);
	virtual ~StringPut();
	virtual bool parse(void);
protected:
	void execChild(void);
};
#endif
