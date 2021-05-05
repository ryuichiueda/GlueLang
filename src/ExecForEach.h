// Copyright 2017 Ryuichi Ueda
// Released under the MIT License.
#ifndef __SUB_FOR_EACH_H_
#define __SUB_FOR_EACH_H_

#include "Executable.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExecForEach : public Executable
{
public:
	ExecForEach(Feeder *f, Environment *env,vector<int> *scopes);
	virtual ~ExecForEach();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f,  DefStr *s);
	string m_name;
//	vector<string> m_script;
	string m_script_path;
};
#endif
