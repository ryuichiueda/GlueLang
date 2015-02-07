#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;
class Envrionment;

class ArgCommand : public Element
{
public:
	ArgCommand(Feeder *f, Environment *env);
	virtual ~ArgCommand();
	const char *getStr(void);

	virtual bool parse(void);

	bool parsePrefixedCom(void);

	void setPrefix(const char *p){m_prefix = string(p);}
//	string *getName(void){return &m_name;}

	bool m_is_proc;
	bool m_is_internal;

	string m_name;
protected:
	string m_prefix;
	string m_path;
	string m_fullpath;

};
#endif
