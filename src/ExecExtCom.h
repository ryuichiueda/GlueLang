// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __EXT_COMMAND_H_
#define __EXT_COMMAND_H_

#include "Executable.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExecExtCom : public Executable
{
public:
	ExecExtCom(Feeder *f, Environment *env, vector<int> *scopes);
	virtual ~ExecExtCom();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefFile *ef, DefStr *s);
};
#endif
