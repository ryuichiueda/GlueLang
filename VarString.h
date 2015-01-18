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

class VarString : public Element
{
public:
	VarString(Feeder *f, Environment *env);
	virtual ~VarString();
	virtual bool parse(void);

	void setName(string s);

	virtual int exec(void);
	virtual bool eval(void);

	//const char *virtualFileName(void){return m_var_name.c_str();}

	int getFd(void){return m_fd;}

	bool readFiFo(void);

	bool m_evaled;
	bool m_opened;
	bool m_is_set;

	Condition* m_condition;
	string m_var_name;
protected:
	string m_file_name;

	int m_fd;
};
#endif
