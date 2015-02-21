#ifndef __VAR_STRING_H_
#define __VAR_STRING_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;
class DefCond;
class DataStr;

class DefStr : public Element
{
public:
	DefStr(Feeder *f, Environment *env);
	virtual ~DefStr();
	virtual bool parse(void);
	virtual int exec(void);

	bool readFifo(void);

	DefCond* m_condition;
	string m_var_name;
protected:
	DataStr *m_data;
};
#endif
