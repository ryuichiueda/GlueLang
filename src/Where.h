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

class Where : public Element
{
public:
	Where(Feeder *f, Environment *env,vector<int> *scopes);;
	virtual ~Where();
	//virtual void print(int indent_level = 0);
	bool parse(void);
	virtual int exec(DefFile *f,  DefStr *s);
	virtual bool eval(void);

	Environment *m_local_env;
};
#endif
