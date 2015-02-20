#ifndef __WHERE_H_
#define __WHERE_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;
class Condition;

class Where : public Element
{
public:
	Where(Feeder *f, Environment *env);
	virtual ~Where();
	//virtual void print(int indent_level = 0);
	bool parse(void);
	virtual int exec(void);
	virtual bool eval(void);

	Condition* findCond(string *var);
	Environment *m_local_env;

protected:
	vector<Condition *> m_conditions;
};
#endif
