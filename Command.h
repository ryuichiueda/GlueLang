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

	bool parsePrefixedCom(void);

	void printOriginalString(void);
	void setPrefix(const char *p){m_prefix = string(p);}
	string *getName(void){return &m_name;}

	bool m_is_proc;
protected:
	string m_prefix;
	string m_name;
	string m_path;
	string m_fullpath;

};
#endif
