// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __ARG_EXT_COM_H_
#define __ARG_EXT_COM_H_

#include "Arg.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class ArgExtCom : public Arg
{
public:
	ArgExtCom(Feeder *f,Environment *env);
	virtual ~ArgExtCom();

	//virtual bool eval(void);
	virtual bool parse(void);
private:
	bool parsePrefixedCom(void);
	string m_prefix;
};
#endif
