// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __SUB_SHELL_H_
#define __SUB_SHELL_H_

#include "Executable.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeSubShell : public Executable
{
public:
	ExeSubShell(Feeder *f, Environment *env,vector<int> *scopes);;
	virtual ~ExeSubShell();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefFile *ef, DefStr *s);
	string m_name;
//	vector<string> m_script;
	string m_script_path;
};
#endif
