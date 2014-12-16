#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "Node.h"
#include <iostream>
using namespace std;

class Feeder;

class Command : public Node
{
public:
	Command(Feeder *f);
	virtual ~Command();
	virtual void print(int indent_level = 0);
	virtual void parser(vector<char> *script,int pos);
protected:
	string m_name;
};
#endif
