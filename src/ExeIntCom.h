// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __INT_COMMAND_H_
#define __INT_COMMAND_H_

#include "Exe.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeIntCom : public Exe
{
public:
	ExeIntCom(Feeder *f, Environment *env,vector<int> *scopes);
	virtual ~ExeIntCom();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefStr *s);
};
#endif
