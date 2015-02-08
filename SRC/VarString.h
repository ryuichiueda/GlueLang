#ifndef __VAR_STRING_H_
#define __VAR_STRING_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;
class Condition;
class StrData;

class VarString : public Element
{
public:
	VarString(Feeder *f, Environment *env);
	virtual ~VarString();
	virtual bool parse(void);
	virtual int exec(void);

	bool readFifo(void);

	Condition* m_condition;
	string m_var_name;
protected:
	StrData *m_data;
};
#endif
