// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __LITERAL_H_
#define __LITERAL_H_

#include "Element.h"
#include "Arg.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class Literal : public Arg
{
public:
	Literal(Feeder *f,Environment *env);
	virtual ~Literal();

	virtual bool eval(void);
	virtual bool parse(void);
protected:
};
#endif
