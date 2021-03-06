// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __ARG_H_
#define __ARG_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class Arg : public Element
{
public:
	Arg(Feeder *f,Environment *env, vector<int> *scopes);
	virtual ~Arg();
	virtual const char *getOriginalString(void){return m_text.c_str();}
	virtual const char *getEvaledString(void){return m_evaled_text.c_str();}

	//virtual bool eval(void) = 0;
	virtual bool parse(void) = 0;

	string m_text;
	string m_evaled_text;
};
#endif
