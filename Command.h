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
