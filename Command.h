#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;

class Command : public Element
{
public:
	Command(Feeder *f);
	virtual ~Command();
	virtual void print(int indent_level = 0);
/*
	const char *getOriginalString(void){return m_text.c_str();}
	const char *getEvaledString(void){return m_evaled_text.c_str();}
*/
	const char *getStr(void);

	virtual bool eval(void);
	virtual int exec(void);
	virtual bool parse(void);

	void printOriginalString(void);
protected:
	string m_text;
//	string m_evaled_text;
};
#endif
