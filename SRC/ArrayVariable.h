// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __ARRAY_VARIABLE_H_
#define __ARRAY_VARIABLE_H_

#include "Arg.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class ArrayVariable : public Arg
{
public:
	ArrayVariable(Feeder *f,Environment *env);
	virtual ~ArrayVariable();

	virtual bool eval(void);
	virtual bool parse(void);
protected:
	long m_array_pos;
};
#endif
