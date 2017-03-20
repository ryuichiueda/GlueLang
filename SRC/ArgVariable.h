// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __ARG_VARIABLE_H_
#define __ARG_VARIABLE_H_

#include "Arg.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class ArgVariable : public Arg
{
public:
	ArgVariable(Feeder *f,Environment *env);
	virtual ~ArgVariable();

	virtual bool eval(void);
	virtual bool parse(void);

	bool m_is_local;
	bool m_is_proc;
};
#endif
