#ifndef __CONDITION_H_
#define __CONDITION_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;
class Envrionment;

class Condition : public Element
{
public:
	Condition(Feeder *f, Environment *env);
	virtual ~Condition();

	virtual bool parse(void);

	string m_name;
	long m_upper_byte;
protected:
	
};
#endif
