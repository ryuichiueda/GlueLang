#ifndef __EXT_COMMAND_H_
#define __EXT_COMMAND_H_

#include "Exe.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeExtCom : public Exe
{
public:
	ExeExtCom(Feeder *f, Environment *env);
	virtual ~ExeExtCom();
	virtual bool parse(void);
protected:
	void execChild(void);
};
#endif