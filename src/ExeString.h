// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __STRING_PUT_H_
#define __STRING_PUT_H_

#include "Exe.h"
#include <iostream>
#include <vector>
using namespace std;

class Feeder;

class ExeString : public Exe
{
public:
	ExeString(Feeder *f,Environment *env, vector<int> *scopes);
	virtual ~ExeString();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefStr *s);
};
#endif
