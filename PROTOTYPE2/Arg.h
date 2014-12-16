#ifndef __ARG_H_
#define __ARG_H_

#include "Node.h"
#include <iostream>
using namespace std;

class Feeder;

class Arg : public Node
{
public:
	Arg(Feeder *f);
	virtual ~Arg();
	virtual void print(int indent_level = 0);
	virtual void parser(vector<char> *script,int pos);
protected:
	string m_text;
};
#endif
