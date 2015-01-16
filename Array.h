#ifndef __ARRAY_H_
#define __ARRAY_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class Array : public Element
{
public:
	Array(Feeder *f,Environment *env);
	virtual ~Array();

	virtual bool eval(void);
	virtual int exec(void);
	virtual bool parse(void);
protected:
};
#endif
