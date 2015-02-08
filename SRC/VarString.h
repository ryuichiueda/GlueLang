#ifndef __VAR_STRING_H_
#define __VAR_STRING_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;
class Arg;
class Condition;
class StrData;

class VarString : public Element
{
public:
	VarString(Feeder *f, Environment *env);
	virtual ~VarString();
	virtual bool parse(void);
	virtual int exec(void);

//	void setName(string s);

	bool readFifo(void);

	bool m_opened;
	bool m_is_set;

	Condition* m_condition;
	string m_var_name;
	string m_file_name;
protected:
	StrData *m_data;
};
#endif
