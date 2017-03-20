// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __PROC_H_
#define __PROC_H_

#include "Element.h"
using namespace std;

class Feeder;
class Envrionment;

class DefProc : public Element
{
public:
	DefProc(Feeder *f, Environment *env);
	virtual ~DefProc();
	//const char *getStr(void);

	virtual bool parse(void);

protected:
	string m_name;
	//vector<string> m_script;
	string m_script_path;
};
#endif
