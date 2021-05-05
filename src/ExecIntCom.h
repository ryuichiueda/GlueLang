// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __INT_COMMAND_H_
#define __INT_COMMAND_H_

#include "Executable.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExecIntCom : public Executable
{
public:
	ExecIntCom(Feeder *f, Environment *env,vector<int> *scopes);
	virtual ~ExecIntCom();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f,  DefStr *s);
};
#endif
