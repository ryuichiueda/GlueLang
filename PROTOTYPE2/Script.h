#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include "Node.h"
using namespace std;

class Feeder;

class Script : public Node
{
public:
	Script(Feeder *feeder);
	virtual ~Script();
	virtual void print(int indent_level = 0);
	virtual void parser(vector<char> *script,int pos);
};
#endif
