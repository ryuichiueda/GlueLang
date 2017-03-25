// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __CONDITION_H_
#define __CONDITION_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;
class Envrionment;

class DefCond : public Element
{
public:
	DefCond(Feeder *f, Environment *env);
	virtual ~DefCond();

	virtual bool parse(void);

	string m_name;
	long m_upper_byte;
protected:
	
};
#endif
