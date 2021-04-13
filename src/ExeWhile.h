// Copyright 2017 Ryuichi Ueda
// Released under the MIT License.
#ifndef __SUB_SHELL_LOOP_H_
#define __SUB_SHELL_LOOP_H_

#include "Executable.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeWhile : public Executable
{
public:
	ExeWhile(Feeder *f, Environment *env,vector<int> *scopes);;
	virtual ~ExeWhile();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefFile *ef, DefStr *s);
	string m_name;
//	vector<string> m_script;
	string m_script_path;
};
#endif
