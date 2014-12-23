#ifndef __ARG_H_
#define __ARG_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;

class Arg : public Element
{
public:
	Arg(string *str,Feeder *f);
	virtual ~Arg();
	virtual void print(int indent_level = 0);
	const char *getOriginalString(void){return m_text.c_str();}
	const char *getEvaledString(void){return m_evaled_text.c_str();}

	virtual int exec(void);

	bool eval(void);
protected:
	string m_text;
	string m_evaled_text;

};
#endif
