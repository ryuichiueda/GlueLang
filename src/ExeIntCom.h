// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __INT_COMMAND_H_
#define __INT_COMMAND_H_

#include "Executable.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeIntCom : public Executable
{
public:
	ExeIntCom(Feeder *f, Environment *env,vector<int> *scopes);
	virtual ~ExeIntCom();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefFile *ef, DefStr *s);
};
#endif
