// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;

class Script : public Element
{
public:
	Script(Feeder *f, Environment *env,vector<int> *scopes);;
	virtual ~Script();
	bool parse(void);
	virtual int exec(DefFile *f, DefStr *s);
	virtual bool eval(void){return true;};

	void setSilent(void){m_silent = true;}
private:
	bool tryElement(Element *n);
	void printErrorMsg(Element *e,string error_type,bool silent = false);
	bool parseScript(void);
	bool m_silent;
	void end(int exit_status);
};
#endif
