#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;

class Script : public Element
{
public:
	Script(Feeder *f, Environment *env);
	virtual ~Script();
	//virtual void print(int indent_level = 0);
	bool parse(void);
	virtual int exec(void);
	virtual bool eval(void){return true;};

	void setSilent(void){m_silent = true;}
protected:
	bool tryElement(Element *n);
	void parseErrorMsg(Element *e);
	void execErrorMsg(Element *e);
	bool doParse(void);
	bool m_silent;
};
#endif
