// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __STRING_PUT_H_
#define __STRING_PUT_H_

#include "Executable.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExecString : public Executable
{
public:
	ExecString(Feeder *f,Environment *env, vector<int> *scopes);
	virtual ~ExecString();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefFile *ef, DefStr *s);
};
#endif
