#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;
class Envrionment;

class Command : public Element
{
public:
	Command(Feeder *f, Environment *env);
	virtual ~Command();
	const char *getStr(void);

	virtual bool eval(void);
	virtual int exec(void);
	virtual bool parse(void);

	void printOriginalString(void);
protected:
	string m_prefix;
	string m_name;
	string m_path;
	string m_full_path;
};
#endif
