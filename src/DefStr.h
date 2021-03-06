// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#ifndef __VAR_STRING_H_
#define __VAR_STRING_H_

#include "Element.h"
#include <iostream>
#include <vector>
using namespace std;

class Element;
class Feeder;
class Environment;
class DataStr;

class DefStr : public Element
{
public:
	DefStr(Feeder *f,Environment *env, vector<int> *scopes);
	virtual ~DefStr();
	virtual bool parse(void);
	void connect(void);

	bool readFifo(void);

	string m_var_name;
protected:
	DataStr *m_data;
};
#endif
