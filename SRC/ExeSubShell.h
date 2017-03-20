// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __SUB_SHELL_H_
#define __SUB_SHELL_H_

#include "Exe.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeSubShell : public Exe
{
public:
	ExeSubShell(Feeder *f, Environment *env);
	virtual ~ExeSubShell();
	virtual bool parse(void);
protected:
	void execChild(void);
	string m_name;
//	vector<string> m_script;
	string m_script_path;
};
#endif
