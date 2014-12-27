#ifndef __IMPORT_H_
#define __IMPORT_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;

class Import : public Element
{
public:
	Import(Feeder *f);
	virtual ~Import();

	virtual bool eval(void);
	virtual int exec(void);
	virtual bool parse(void);

	void printOriginalString(void);
protected:
	string m_path;
	string m_prefix;
};
#endif
