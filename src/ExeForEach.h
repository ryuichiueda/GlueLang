// Copyright 2017 Ryuichi Ueda
// Released under the MIT License.
#ifndef __SUB_FOR_EACH_H_
#define __SUB_FOR_EACH_H_

#include "Executable.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeForEach : public Executable
{
public:
	ExeForEach(Feeder *f, Environment *env,vector<int> *scopes);
	virtual ~ExeForEach();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefFile *ef, DefStr *s);
	string m_name;
//	vector<string> m_script;
	string m_script_path;
};
#endif
