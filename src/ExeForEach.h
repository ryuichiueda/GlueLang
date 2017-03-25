// Copyright 2017 Ryuichi Ueda
// Released under the MIT License.
#ifndef __SUB_FOR_EACH_H_
#define __SUB_FOR_EACH_H_

#include "Exe.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeForEach : public Exe
{
public:
	ExeForEach(Feeder *f, Environment *env);
	virtual ~ExeForEach();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefFile *ef, DefStr *s);
	string m_name;
//	vector<string> m_script;
	string m_script_path;
};
#endif
