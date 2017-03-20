// Copyright 2017 Ryuichi Ueda
// Released under the MIT License.
#ifndef __SUB_SHELL_LOOP_H_
#define __SUB_SHELL_LOOP_H_

#include "Exe.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeSubShellLoop : public Exe
{
public:
	ExeSubShellLoop(Feeder *f, Environment *env);
	virtual ~ExeSubShellLoop();
	virtual bool parse(void);
protected:
	void execChild(void);
	string m_name;
//	vector<string> m_script;
	string m_script_path;
};
#endif
