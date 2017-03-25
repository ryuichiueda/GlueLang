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
	ExeString(Feeder *f, Environment *env);
	virtual ~ExeString();
	virtual bool parse(void);
protected:
	void execChild(DefFile *f, DefFile *ef, DefStr *s, DefStr *es);
};
#endif
