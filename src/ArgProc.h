// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __ARG_PROC_H_
#define __ARG_PROC_H_

#include "Arg.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class ArgProc : public Arg
{
public:
	ArgProc(Feeder *f,Environment *env,vector<int> *scopes);;
	virtual ~ArgProc();

	virtual bool eval(void);
	virtual bool parse(void);
};
#endif
