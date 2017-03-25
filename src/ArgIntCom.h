// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __ARG_INT_COM_H_
#define __ARG_INT_COM_H_

#include "Arg.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class ArgIntCom : public Arg
{
public:
	ArgIntCom(Feeder *f,Environment *env,vector<int> *scopes);;
	virtual ~ArgIntCom();

//	virtual bool eval(void);
	virtual bool parse(void);
};
#endif
