#ifndef __PROC_H_
#define __PROC_H_

#include "Element.h"
#include <iostream>
using namespace std;

class Feeder;
class Envrionment;

class Procedure : public Element
{
public:
	Procedure(Feeder *f, Environment *env);
	virtual ~Procedure();
	//const char *getStr(void);

	virtual bool eval(void);
	virtual int exec(void);
	virtual bool parse(void);

protected:
	string m_name;
	vector<string> m_script;
	string m_script_path;
};
#endif
