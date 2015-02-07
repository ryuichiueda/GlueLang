#ifndef __ARRAY_H_
#define __ARRAY_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;
class Environment;

class StringArray : public Element
{
public:
	StringArray(Feeder *f,Environment *env);
	virtual ~StringArray();

	virtual bool eval(void);
	virtual bool parse(void);

	const char** makeArgv(void);

	void connect(string delimiter);
protected:

	string m_com;
	string m_connected;
};
#endif
