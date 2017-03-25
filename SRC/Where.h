// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __WHERE_H_
#define __WHERE_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;
class DefCond;

class Where : public Element
{
public:
	Where(Feeder *f, Environment *env);
	virtual ~Where();
	//virtual void print(int indent_level = 0);
	bool parse(void);
	virtual int exec(DefFile *f, DefFile *ef, DefStr *s, DefStr *es);
	virtual bool eval(void);

	DefCond* findCond(string *var);
	Environment *m_local_env;
protected:
	vector<DefCond *> m_conditions;
};
#endif
