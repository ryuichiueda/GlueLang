#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;

class Script : public Element
{
public:
	Script(Feeder *f);
	virtual ~Script();
	//virtual void print(int indent_level = 0);
	bool parse(void);
	virtual int exec(void);
	virtual bool eval(void){return true;};
protected:

	bool tryElement(Element *n);
};
#endif
